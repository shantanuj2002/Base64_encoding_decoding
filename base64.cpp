#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

class Base64Converter {
public:
    Base64Converter(const string& input_file_path, const string& output_file_path)
        : input_file_path_(input_file_path), output_file_path_(output_file_path) {}

    void encode() {
        ifstream input_file(input_file_path_, ios::binary);
        if (!input_file.is_open()) {
            cerr << "Error opening input file." << endl;
            exit(1);
        }

        stringstream buffer;
        buffer << input_file.rdbuf();
        string binary_data_str = buffer.str();

        // Encoding
        encoded_data_ = base64_encode(
            vector<unsigned char>(binary_data_str.begin(), binary_data_str.end())
        );
    }

    void saveEncodedData() {
        ofstream output_file(output_file_path_);
        output_file << encoded_data_;
        output_file.close();
        cout << "Data encoding completed." << endl;
    }

    void decode() {
        // Decoding
        vector<unsigned char> decoded_data = base64_decode(encoded_data_);

        // Save decoded data to a binary file
        ofstream output_file("decode.bin", ios::binary);
        output_file.write(reinterpret_cast<char*>(&decoded_data[0]), decoded_data.size());
        output_file.close();
        cout << "Data decoding completed." << endl;
    }

private:
    const string input_file_path_;
    string output_file_path_;
    string encoded_data_;

    string base64_encode(const vector<unsigned char>& binary_data) {
        const string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        string encoded;
        size_t i = 0;

        while (i < binary_data.size()) {
            unsigned char input[3] = {0};
            unsigned char output[4] = {0};

            for (int j = 0; j < 3 && i < binary_data.size(); ++j) {
                input[j] = binary_data[i++];
            }

            output[0] = (input[0] & 0xFC) >> 2;
            output[1] = ((input[0] & 0x03) << 4) | ((input[1] & 0xF0) >> 4);
            output[2] = ((input[1] & 0x0F) << 2) | ((input[2] & 0xC0) >> 6);
            output[3] = input[2] & 0x3F;

            for (int j = 0; j < 4; ++j) {
                encoded += base64_chars[output[j]];
            }
        }

        return encoded;
    }

    vector<unsigned char> base64_decode(const string& encoded_data) {
        const string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        vector<unsigned char> binary_data;
        size_t i = 0;

        while (i < encoded_data.length()) {
            unsigned char input[4] = {0};
            unsigned char output[3] = {0};

            for (int j = 0; j < 4 && i < encoded_data.length(); ++j) {
                while (i < encoded_data.length() &&
                       base64_chars.find(encoded_data[i]) == string::npos) {
                    i++;
                }
                input[j] = encoded_data[i++];
            }

            output[0] = (input[0] << 2) | ((input[1] & 0x30) >> 4);
            output[1] = ((input[1] & 0x0F) << 4) | ((input[2] & 0x3C) >> 2);
            output[2] = ((input[2] & 0x03) << 6) | (input[3] & 0x3F);

            for (int j = 0; j < 3; ++j) {
                binary_data.push_back(output[j]);
            }
        }

        return binary_data;
    }
};

int main() {
    Base64Converter converter("sample.txt", "encoded_data.txt");
    converter.encode();
    converter.saveEncodedData();

    // Perform decoding 
    converter.decode();

    return 0;
}
