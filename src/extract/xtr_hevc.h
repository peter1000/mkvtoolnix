/*
   mkvextract -- extract tracks from Matroska files into other files

   Distributed under the GPL v2
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   extracts tracks from Matroska files into other files

*/

#ifndef MTX_XTR_HEVC_H
#define MTX_XTR_HEVC_H

#include "common/common_pch.h"

#include "extract/xtr_base.h"
#include "extract/xtr_avc.h"

class xtr_hevc_c: public xtr_avc_c {
protected:
  bool m_first_nalu{true};

public:
  xtr_hevc_c(const std::string &codec_id, int64_t tid, track_spec_t &tspec) : xtr_avc_c(codec_id, tid, tspec) { }

  virtual const char *get_container_name() {
    return "HEVC/h.265 elementary stream";
  };

  virtual void create_file(xtr_base_c *master, KaxTrackEntry &track);

  virtual bool write_nal(const binary *data, size_t &pos, size_t data_size, size_t write_nal_size_size);

};

#endif
