#ifndef ABSTRACT_ROW_KEY_ENCODER
#define ABSTRACT_ROW_KEY_ENCODER

#include <memory>
#include <vector>
#include <string>

#include "core-metadata/metadata/model/cube_desc.hpp"
#include "core-cube/kv/row_key_encoder.hpp"

namespace husky {
namespace cube {

class AbstractRowKeyEncoder
{
public:
	static const unsigned char DEFUAL_BLANK_BYTE = 0xff;
	inline AbstractRowKeyEncoder * create_instance(CubeDesc cube_desc, Cuboid * cuboid) {
		return new RowkeyEncoder(cube_desc, cuboid);
	}
	inline void set_blank_byte(unsigned char blank_byte) {
		blank_byte_ = blank_byte;
	}
	inline uint64_t get_cuboid_id() const {
		return cuboid_.get_id();
	}
	inline void set_cuboid_id(Cuboid * cuboid) {
		cuboid_ = cuboid;
	}

	// virtual std::vector<unsigned char> creat_buf() = 0;
	virtual void encode(const std::vector<unsigned char> & body_bytes, std::vector<unsigned char> & output_buf) = 0;
	virtual std::vector<unsigned char> encode(std::map<TblColRef *, std::string> & value_map) = 0;
	virtual std::vector<unsigned char> encode(std::vector<std::string> & values) = 0;

protected:
	unsigned char blank_byte_ = DEFUAL_BLANK_BYTE;
	std::shared_ptr<CubeDesc> cube_desc_;
	Cuboid * cuboid_;
	AbstractRowKeyEncoder(std::shared_ptr<CubeDesc> cube_desc, Cuboid * cuboid):cube_desc_(cube_desc), cuboid_(cuboid) {}
	~AbstractRowKeyEncoder() {}
	
};

}  // namespace cube
}  // namespace husky

#endif