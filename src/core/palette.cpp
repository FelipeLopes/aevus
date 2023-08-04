#include "palette.hpp"
#include <optional>

using std::optional;
using std::string;
using std::vector;

namespace core {

PaletteColors::PaletteColors() { }

char PaletteColors::hexHigh(byte b) const {
    return b/16 > 9 ? (b/16)+'A'-10 : (b/16)+'0';
}

char PaletteColors::hexLow(byte b) const {
    return b%16 > 9 ? (b%16)+'A'-10 : (b%16)+'0';
}

PaletteColors::byte PaletteColors::hexValue(char high, char low) const {
    return (high >= 'A' ? (10 + high - 'A') : (high - '0')) * 16
        + (low >= 'A' ? (10 + low - 'A') : (low - '0'));
}

string PaletteColors::hexAt(int pos) const {
    char s[7];
    byte r = paletteData[3*pos];
    byte g = paletteData[3*pos+1];
    byte b = paletteData[3*pos+2];
    s[0] = hexHigh(r);
    s[1] = hexLow(r);
    s[2] = hexHigh(g);
    s[3] = hexLow(g);
    s[4] = hexHigh(b);
    s[5] = hexLow(b);
    s[6] = '\0';
    return std::string(s);
}

void PaletteColors::readColorCLArray(vector<ColorCL>& arr) const {
    arr.resize(PALETTE_WIDTH);
    for (int i=0; i<PALETTE_WIDTH; i++) {
        arr[i].r = paletteData[3*i] / 255.0;
        arr[i].g = paletteData[3*i+1] / 255.0;
        arr[i].b = paletteData[3*i+2] / 255.0;
        arr[i].a = 1.0;
    }
}

optional<string> PaletteColors::toString() {
    string whiteSpace(8, ' ');
    string text = "\n";
    for (int i=0; i<32; i++) {
        string paletteChars = "";
        for (int j=0; j<8; j++) {
            paletteChars += hexAt(i*8+j);
        }
        text += (whiteSpace + paletteChars + "\n");
    }
    text += "    ";
    return text;
}

void PaletteColors::fromString(optional<string> text) {
    if (!text.has_value()) {
        throw std::invalid_argument("Palette color string does not exist");
    }
    auto str = text.value();
    str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.size() != PALETTE_WIDTH * 6) {
        throw std::invalid_argument("Palette color string has wrong size");
    }
    paletteData.resize(PALETTE_WIDTH * 3);
    for (int i=0; i<PALETTE_WIDTH; i++) {
        paletteData[3*i] = hexValue(str[6*i], str[6*i+1]);
        paletteData[3*i+1] = hexValue(str[6*i+2], str[6*i+3]);
        paletteData[3*i+2] = hexValue(str[6*i+4], str[6*i+5]);
    }
}

Palette::Palette(XMLElementClass& el): XMLElementClass(el, "palette"),
    count(*this, "count"),
    format(*this, "format"),
    colors(*this)
{
    count.setValue(256);
    format.setValue("RGB");
}

void Palette::readColorCLArray(vector<ColorCL>& arr) const {
    colors.getValue().readColorCLArray(arr);
}

}