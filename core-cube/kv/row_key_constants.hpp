#ifndef ROW_KEY_CONSTANTS
#define ROW_KEY_CONSTANTS

class RowKeyConstants
{
public:
	static const int ROWKEY_COL_DEFAULT_LENGTH = 256;
	static const int ROWKEY_CUBOIDID_LENGTH = 8; // uint_64t
	static const int ROWKEY_BUFFER_SIZE = 65 * 256; // a little more than 64 dimensions * 256 bytes each
};

#endif