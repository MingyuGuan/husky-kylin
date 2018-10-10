#include "core-cube/kv/row_key_encoder.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

RowKeyEncoder::RowKeyEncoder(cubeDesc cube_desc, Cuboid * cuboid): AbstractRowKeyEncoder(cube_desc, cuboid) {
	for(auto const & column : cuboid.getColumns()) {
		body_length_ += 4; // hard code! Should be different by dimension type (int, data, string, ...)
	}
}

void RowKeyEncoder::encode(const std::vector<unsigned char> & body_bytes, std::vector<unsigned char> & output_buf) {
	output_buf = body_bytes;
	fill_header(output_buf);
}

std::vector<unsigned char> RowKeyEncoder::encode(std::map<TblColRef *, std::string> & value_map) {
	std::vector<TblColRef *> columns = cuboid_.getColumns();
	std::vector<std::string> values;
	for(auto const & column : columns) {
		values.push_back(value_map.find(column)->second);
	}
	return encode(values);
}

std::vector<unsigned char> RowKeyEncoder::encode(std::vector<std::string> & values) {
	std::vector<unsigned char> bytes;
	int offset = get_header_length();
	std::vector<TblColRef *> columns = cuboid_.getColumns();

	for(int i = 0; i < columns.size(); i++) {
		TblColRef * column = columns[i];
		// int colLength = colIO.getColumnLength(column); // hard code!
		int col_length = 4;
		fill_column_value(column, col_length, values[i], bytes, offset);
		offset += col_length;
	}

	fill_header(bytes);
	return bytes;
}

}  // namespace cube
}  // namespace husky