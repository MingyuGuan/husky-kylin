// Copyright 2018 Husky Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <memory>
#include <vector>
#include <set>

#include "core-cube/cuboid/cuboid_scheduler.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"
#include "utils/utils.hpp"

namespace husky {
namespace cube {

class Cuboid {
   public:
    Cuboid(const std::shared_ptr<CubeDesc>& cube_desc, uint64_t original_id, uint64_t valid_id);
    ~Cuboid() {}

    inline static Cuboid * find_cuboid(CuboidScheduler cuboid_scheduler, std::set<TblColRef *> dimensions) {
      uint64_t cuboid_id = to_cuboid_id(cuboid_scheduler.get_cube_desc(), dimensions);
      return Cuboid::find_by_id(cuboid_scheduler, cuboid_id);
    }
    inline static Cuboid * find_by_id(CuboidScheduler cuboid_scheduler, const std::vector<unsigned char> & cuboid_id) {
      return find_by_id(cuboid_scheduler, utils::bytes_to_long(id_));
    }
    inline static Cuboid * find_by_id(CuboidScheduler cuboid_scheduler, uint64_t cuboid_id) {
      uint64_t valid_cuboid_id = cuboid_scheduler.find_best_match_cuboid(cuboid_id);
      Cuboid * cuboid = new (cuboid_scheduler.get_cube_desc(), cuboid_id, valid_cuboid_id);
      return cuboid;
    }
    static uint64_t to_cuboid_id(CubeDesc * cube_desc, std::set<TblColRef *> dimensions);
    inline static long get_base_cuboid_id(CubeDesc * cube) {
      return cube.get_row_key().get_full_mask();
    }
    inline static Cuboid * get_base_cuboid_id(CubeDesc * cube) {
      return find_by_id(cube.get_initial_cuboid_scheduler(), get_base_cuboid_id(cube));
    }

    inline std::shared_ptr<CubeDesc> get_cube_desc() const { return cube_desc_;}
    inline std::vector<TblColRef> get_columns() const { return dimension_columns_; }
    inline uint64_t get_id const { return id_; }
    inline std::vector<unsigned char> get_bytes() const { return id_bytes_; }
    inline uint64_t get_input_id() const { return input_id_; }

   protected: 
    std::vector<TblColRef*> translated_id_to_columns(uint64_t cuboid_id);

   private:
    std::shared_ptr<CubeDesc> cube_desc_;
    uint64_t input_id_;
    uint64_t id_;
    std::vector<unsigned char> id_bytes_;
    std::vector<TblColRef*> dimension_columns_;
};

}  // namespace cube
}  // namespace husky
