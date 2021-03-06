/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   class definition for the generic reader and packetizer

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef MTX_MERGE_GENERIC_READER_H
#define MTX_MERGE_GENERIC_READER_H

#include "common/common_pch.h"

#include <boost/optional.hpp>

#include "common/chapters/chapters.h"
#include "common/translation.h"
#include "merge/file_status.h"
#include "merge/id_result.h"
#include "merge/packet.h"
#include "merge/timecode_factory.h"
#include "merge/track_info.h"
#include "merge/webm.h"

using namespace libmatroska;

class generic_packetizer_c;

#define DEFTRACK_TYPE_AUDIO 0
#define DEFTRACK_TYPE_VIDEO 1
#define DEFTRACK_TYPE_SUBS  2

#define PTZR(i) m_reader_packetizers[i]
#define PTZR0   PTZR(0)
#define NPTZR() m_reader_packetizers.size()

#define show_demuxer_info() \
  if (verbose) \
    mxinfo_fn(m_ti.m_fname, boost::format(Y("Using the demultiplexer for the format '%1%'.\n")) % get_format_name());
#define show_packetizer_info(track_id, packetizer) \
  mxinfo_tid(m_ti.m_fname, track_id, boost::format(Y("Using the output module for the format '%1%'.\n")) % packetizer->get_format_name());

class generic_reader_c {
public:
  track_info_c m_ti;
  mm_io_cptr m_in;
  uint64_t m_size;

  std::vector<generic_packetizer_c *> m_reader_packetizers;
  generic_packetizer_c *m_ptzr_first_packet;
  std::vector<int64_t> m_requested_track_ids, m_available_track_ids, m_used_track_ids;
  int64_t m_max_timecode_seen;
  kax_chapters_cptr m_chapters;
  bool m_appending;
  int m_num_video_tracks, m_num_audio_tracks, m_num_subtitle_tracks;

  int64_t m_reference_timecode_tolerance;

protected:
  id_result_t m_id_results_container;
  std::vector<id_result_t> m_id_results_tracks, m_id_results_attachments, m_id_results_chapters, m_id_results_tags;

  timecode_c m_restricted_timecodes_min, m_restricted_timecodes_max;

public:
  generic_reader_c(const track_info_c &ti, const mm_io_cptr &in);
  virtual ~generic_reader_c();

  virtual translatable_string_c get_format_name() const = 0;
  virtual bool is_providing_timecodes() const {
    return true;
  }

  virtual void set_timecode_restrictions(timecode_c const &min, timecode_c const &max);
  virtual timecode_c const &get_timecode_restriction_min() const;
  virtual timecode_c const &get_timecode_restriction_max() const;

  virtual void read_headers() = 0;
  virtual file_status_e read(generic_packetizer_c *ptzr, bool force = false) = 0;
  virtual void read_all();
  virtual int get_progress();
  virtual void set_headers();
  virtual void set_headers_for_track(int64_t tid);
  virtual void identify() = 0;
  virtual void create_packetizer(int64_t tid) = 0;
  virtual void create_packetizers() {
    create_packetizer(0);
  }

  virtual int add_packetizer(generic_packetizer_c *ptzr);
  virtual size_t get_num_packetizers() const;
  virtual generic_packetizer_c *find_packetizer_by_id(int64_t id) const;
  virtual void set_timecode_offset(int64_t offset);

  virtual void check_track_ids_and_packetizers();
  virtual void add_requested_track_id(int64_t id);
  virtual void add_available_track_id(int64_t id);
  virtual void add_available_track_ids();
  virtual void add_available_track_id_range(int64_t start, int64_t end);
  virtual void add_available_track_id_range(int64_t num) {
    add_available_track_id_range(0, num - 1);
  }

  virtual int64_t get_file_size() {
    return m_in->get_size();
  }
  virtual int64_t get_queued_bytes() const;
  virtual bool is_simple_subtitle_container() {
    return false;
  }

  virtual file_status_e flush_packetizer(int num);
  virtual file_status_e flush_packetizer(generic_packetizer_c *ptzr);
  virtual file_status_e flush_packetizers();

  virtual attach_mode_e attachment_requested(int64_t id);

  virtual void display_identification_results();

protected:
  virtual bool demuxing_requested(char type, int64_t id, std::string const &language = "");

  virtual void id_result_container(const std::string &verbose_info = empty_string);
  virtual void id_result_container(const std::vector<std::string> &verbose_info);
  virtual void id_result_track(int64_t track_id, const std::string &type, const std::string &info, const std::string &verbose_info = empty_string);
  virtual void id_result_track(int64_t track_id, const std::string &type, const std::string &info, const std::vector<std::string> &verbose_info);
  virtual void id_result_attachment(int64_t attachment_id, const std::string &type, int size, const std::string &file_name = empty_string, const std::string &description = empty_string,
                                    boost::optional<uint64_t> id = boost::optional<uint64_t>{});
  virtual void id_result_chapters(int num_entries);
  virtual void id_result_tags(int64_t track_id, int num_entries);

  virtual std::string id_escape_string(const std::string &s);

  virtual mm_io_c *get_underlying_input() const;
};

#endif  // MTX_MERGE_GENERIC_READER_H
