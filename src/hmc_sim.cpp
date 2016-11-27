#include <iostream>
#include <cassert>
#include "hmc_cube.h"
#include "hmc_sim.h"
#include "hmc_link.h"
#include "hmc_quad.h"
#include "hmc_queue.h"
#include "hmc_vault.h"

hmc_sim::hmc_sim(unsigned num_hmcs, unsigned num_slids,
                 unsigned num_links, unsigned capacity,
                 enum link_width_t ringbuswidth,
                 enum link_width_t vaultbuswidth) :
  clk(0),
  cubes_notify(0, nullptr, this)
{
  if ((num_hmcs > HMC_MAX_DEVS) || (!num_hmcs)) {
    std::cerr << "INSUFFICIENT NUMBER DEVICES: between 1 to " << HMC_MAX_DEVS << " (" << num_hmcs << ")" << std::endl;
    throw false;
  }

  switch (num_links) {
  case HMC_MIN_LINKS:
  case HMC_MAX_LINKS:
    break;
  default:
    std::cerr << "INSUFFICIENT NUMBER LINKS: between " << HMC_MIN_LINKS << " to " << HMC_MAX_LINKS << " (" << num_links << ")" << std::endl;
    throw false;
  }

  if (num_slids >= HMC_MAX_LINKS) {
    std::cerr << "INSUFFICIENT NUMBER SLIDS: between " << HMC_MIN_LINKS << " to " << HMC_MAX_LINKS << " (" << num_links << ")" << std::endl;
    throw false;
  }

  switch (capacity) {
  case HMC_MIN_CAPACITY:
  case HMC_MAX_CAPACITY:
    break;
  default:
    std::cerr << "INSUFFICIENT AMOUNT CAPACITY: between " << HMC_MIN_CAPACITY << " to " << HMC_MAX_CAPACITY << " (" << capacity << ")" << std::endl;
    throw false;
  }

  for (unsigned i = 0; i < num_slids; i++) {
    this->slidnotify[i] = new hmc_notify(i, nullptr, nullptr);
  }

  for (unsigned i = 0; i < num_hmcs; i++) {
    this->cubes[i] = new hmc_cube(i, &this->cubes_notify, ringbuswidth, vaultbuswidth, capacity, &this->cubes, num_hmcs);
    this->jtags[i] = new hmc_jtag(this->cubes[i]);
  }
}

hmc_sim::~hmc_sim(void)
{
  for (auto it = this->slidnotify.begin(); it != this->slidnotify.end(); ++it) {
    delete (*it).second;
  }

  unsigned i = 0;
  for (auto it = this->cubes.begin(); it != this->cubes.end(); ++it) {
    delete (*it).second;
    delete this->jtags[i++];
  }

  for (auto it = this->link_garbage.begin(); it != this->link_garbage.end(); ++it) {
    delete[] *it;
  }
}

bool hmc_sim::notify_up(void)
{
  return true; // don't care
}

bool hmc_sim::hmc_set_link_config(unsigned src_hmcId, unsigned src_linkId,
                                  unsigned dst_hmcId, unsigned dst_linkId,
                                  enum link_width_t bitwidth)
{
  hmc_link *link = new hmc_link[2];
  link[0].connect_linkports(&link[1]);
  link[0].re_adjust_links(bitwidth, 1);

  hmc_quad *src_quad = this->cubes[src_hmcId]->get_quad(src_linkId);
  hmc_quad *dst_quad = this->cubes[dst_hmcId]->get_quad(dst_linkId);

  bool ret = src_quad->set_ext_link(&link[0]);
  ret &= dst_quad->set_ext_link(&link[1]);
  if (ret) {
    this->cubes[src_hmcId]->get_partial_link_graph(dst_hmcId)->links |= (0x1 << src_linkId);
    this->cubes[dst_hmcId]->get_partial_link_graph(src_hmcId)->links |= (0x1 << dst_linkId);
    this->cubes[src_hmcId]->hmc_routing_tables_update(); // just one needed ...
    this->cubes[src_hmcId]->hmc_routing_tables_visualize();

    this->link_garbage.push_back(link);
    return true;
  }
  else {
    delete[] link;
    return false;
  }
}

hmc_notify* hmc_sim::hmc_define_slid(unsigned slidId, unsigned hmcId, unsigned linkId, enum link_width_t bitwidth)
{
  hmc_quad *quad = this->cubes[hmcId]->get_quad(linkId);

  hmc_link *link = new hmc_link[2];
  link[0].connect_linkports(&link[1]);
  link[1].re_adjust_links(bitwidth, 1);
  link[1].set_ilink_notify(slidId, this->slidnotify[slidId]); // important 1!! -> will be return for slid

  // notify all!
  for (unsigned i = 0; i < this->cubes.size(); i++) {
    this->cubes[i]->set_slid(slidId, hmcId, linkId);
  }

  if (quad->set_ext_link(&link[0])) {
    this->link_garbage.push_back(link);
    this->slids[slidId] = &link[1];
    return this->slidnotify[slidId];
  }
  else {
    delete[] link;
    return nullptr;
  }
}

bool hmc_sim::hmc_send_pkt(unsigned slidId, void *pkt)
{
  uint64_t header = HMC_PACKET_HEADER(pkt);

  assert(HMCSIM_PACKET_REQUEST_GET_CUB(header) < this->cubes.size());
  assert(this->slids.find(slidId) != this->slids.end());

  unsigned flits = HMCSIM_PACKET_REQUEST_GET_LNG(header);
  unsigned len64bit = flits << 1;
  hmc_queue *slid = this->slids[slidId]->get_olink();
  if (!slid->has_space(flits * FLIT_WIDTH)) // check if we have space!
    return false;

  uint64_t *packet = new uint64_t[len64bit];
  memcpy(packet, pkt, len64bit * sizeof(uint64_t));
  packet[0] |= HMCSIM_PACKET_SET_REQUEST();

  packet[len64bit - 1] &= ~(uint64_t)HMCSIM_PACKET_REQUEST_SET_SLID(~0x0); // mask out whatever is set for slid
  packet[len64bit - 1] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_SLID(slidId); // set slidId

  return slid->push_back(packet, flits * FLIT_WIDTH);
}

bool hmc_sim::hmc_recv_pkt(unsigned slidId, void *pkt)
{
  assert(this->slids.find(slidId) != this->slids.end());
  assert(pkt != nullptr);

  unsigned recvpacketleninbit;
  uint64_t *packet = (uint64_t*)this->slids[slidId]->get_ilink()->front(&recvpacketleninbit);
  if (packet == nullptr)
    return false;

  this->slids[slidId]->get_ilink()->pop_front();
  memcpy(pkt, packet, recvpacketleninbit / 64);
  delete[] packet;
  return true;
}

void hmc_sim::hmc_decode_pkt(void *packet, uint64_t *response_head, uint64_t *response_tail,
                             hmc_response_t *type, unsigned *rtn_flits, uint16_t *tag,
                             uint8_t *slid, uint8_t *rrp, uint8_t *frp, uint8_t *seq,
                             uint8_t *dinv, uint8_t *errstat, uint8_t *rtc, uint32_t *crc)
{
  uint64_t header = HMC_PACKET_HEADER(packet);

  uint8_t flits = (uint8_t)HMCSIM_PACKET_RESPONSE_GET_LNG(header);
  uint64_t tail = ((uint64_t*)packet)[ (flits << 1) - 1 ];

  if (response_head != NULL)
    *response_head = header;
  if (response_tail != nullptr)
    *response_tail = tail;

  if (type != nullptr)
    *type = (hmc_response_t)HMCSIM_PACKET_RESPONSE_GET_CMD(header);
  if (rtn_flits != nullptr)
    *rtn_flits = flits;
  if (tag != nullptr)
    *tag = (uint16_t)HMCSIM_PACKET_RESPONSE_GET_TAG(header);
  if (slid != nullptr)
    *slid = (uint8_t)HMCSIM_PACKET_RESPONSE_GET_SLID(header);

  if (rrp != nullptr)
    *rrp = (uint8_t)HMCSIM_PACKET_RESPONSE_GET_RRP(tail);
  if (frp != nullptr)
    *frp = (uint8_t)HMCSIM_PACKET_RESPONSE_GET_FRP(tail);
  if (seq != nullptr)
    *seq = (uint8_t)HMCSIM_PACKET_RESPONSE_GET_SEQ(tail);
  if (dinv != nullptr)
    *dinv = (uint8_t)HMCSIM_PACKET_RESPONSE_GET_DINV(tail);
  if (errstat != nullptr)
    *errstat = (uint8_t)HMCSIM_PACKET_RESPONSE_GET_ERRSTAT(tail);
  if (rtc != nullptr)
    *rtc = (uint8_t)HMCSIM_PACKET_RESPONSE_GET_RTC(tail);
  if (crc != nullptr)
    *crc = (uint32_t)HMCSIM_PACKET_RESPONSE_GET_CRC(tail);
}

void hmc_sim::hmc_encode_pkt(unsigned cub, uint64_t addr,
                             uint16_t tag, hmc_rqst_t cmd, void *packet)
{
  unsigned flits = 0;
  switch (cmd) { // ToDo: would be nice, if their would be something common -> see hmc_process_packet.h
  case RD16:
  case RD32:
  case RD48:
  case RD64:
  case RD80:
  case RD96:
  case RD112:
  case RD128:
  case RD256:
  case MD_RD:
  case FLOW_NULL:   // right?
  case PRET:
  case TRET:
  case IRTRY:
  case INC8:
  case P_INC8:
    flits = 1;
    break;
  case WR16:
  case MD_WR:
  case BWR:
  case TWOADD8:
  case ADD16:
  case TWOADDS8R:
  case ADDS16R:
  case XOR16:
  case OR16:
  case NOR16:
  case AND16:
  case NAND16:
  case CASGT8:
  case CASGT16:
  case CASLT8:
  case CASLT16:
  case CASEQ8:
  case CASZERO16:
  case EQ8:
  case EQ16:
  case BWR8R:
  case SWAP16:
  case P_WR16:
  case P_BWR:
  case P_2ADD8:
  case P_ADD16:
    flits = 2;
    break;
  case WR32:
  case P_WR32:
    flits = 3;
    break;
  case WR48:
  case P_WR48:
    flits = 4;
    break;
  case WR64:
  case P_WR64:
    flits = 5;
    break;
  case WR80:
  case P_WR80:
    flits = 6;
    break;
  case WR96:
  case P_WR96:
    flits = 7;
    break;
  case WR112:
  case P_WR112:
    flits = 8;
    break;
  case WR128:
  case P_WR128:
    flits = 9;
    break;
  case WR256:
  case P_WR256:
    flits = 17;
    break;
  default:
    // ToDo: CMC!
    throw false;
  }

  uint64_t *pkt = (uint64_t*)packet;
  pkt[0] = 0x0ull;
  pkt[0] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_CMD(cmd);
  pkt[0] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_LNG(flits);
  pkt[0] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_TAG(tag);
  pkt[0] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_ADRS(addr);
  pkt[0] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_CUB(cub);

  pkt[2 * flits - 1] = 0x00ull;
  pkt[2 * flits - 1] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_RRP(hmcsim_rqst_getrrp());
  pkt[2 * flits - 1] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_FRP(hmcsim_rqst_getfrp());
  pkt[2 * flits - 1] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_SEQ(hmcsim_rqst_getseq(cmd));
  pkt[2 * flits - 1] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_Pb(0x1);
  //pkt[2 * flits - 1] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_SLID(slid);
  pkt[2 * flits - 1] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_RTC(hmcsim_rqst_getrtc());
  pkt[2 * flits - 1] |= (uint64_t)HMCSIM_PACKET_REQUEST_SET_CRC(hmcsim_crc32((unsigned char*)pkt, 2 * flits));  // crc32 calc. needs to be last of packet init!
}

void hmc_sim::clock(void)
{
  this->clk++;
  uint32_t notifymap = this->cubes_notify.get_notification();
  unsigned lid = __builtin_ctzl(notifymap);
  for (unsigned h = lid; h < this->cubes.size(); h++) {
    if ((0x1 << h) & notifymap) {
      this->cubes[h]->clock();
    }
  }
}
