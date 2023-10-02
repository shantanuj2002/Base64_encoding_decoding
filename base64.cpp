#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdlib> // For exit()

using namespace std;

// Function to encode binary data into Base64
string base64_encode(const vector<unsigned char> &binary_data) {
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

// Function to decode Base64 data back to binary
vector<unsigned char> base64_decode(const string &encoded_data) {
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

int main() {
    ifstream mp3_file("audio.mp3", ios::binary); // Replace with the path to your MP3 file

    if (!mp3_file.is_open()) {
        cerr << "Error opening MP3 file." << endl;
        return 1;
    }

    stringstream buffer;
    buffer << mp3_file.rdbuf();
    string binary_mp3_data_str = buffer.str();

    // Encoding MP3 data
    string encoded_mp3_data = base64_encode(
        vector<unsigned char>(binary_mp3_data_str.begin(), binary_mp3_data_str.end())
    );

    // Save encoded MP3 data to a file
    ofstream encoded_mp3_file("encoded1_audio.txt");
    encoded_mp3_file << encoded_mp3_data;
    encoded_mp3_file.close();

    cout << "MP3 encoding completed." << endl;

    // Decoding Base64 data
    vector<unsigned char> decoded_data = base64_decode(encoded_mp3_data);
    // Save decoded data as an MP3 file
    ofstream decoded_mp3_file("decoded_audio.mp3", ios::binary);
    decoded_mp3_file.write(reinterpret_cast<char*>(&decoded_data[0]), decoded_data.size());
    decoded_mp3_file.close();

    cout << "MP3 decoding completed." << endl;

    return 0;
}
