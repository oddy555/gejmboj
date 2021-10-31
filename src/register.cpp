#include "../inc/register.hpp"
#include <string>

Register8::Register8() {
    data = 255;
    name = "NA";
}    
Register8::Register8(uint8_t data, std::string name) {
    this->data = data;
    this->name = name;
}

uint8_t Register8::getData() {
    return data;
}

std::string Register8::getName () {
    return name;
}

void Register8::setData(uint8_t data)
{
    this->data = data;
}

void Register8::setName(std::string name) {
    this->name = name;
}

Register16::Register16() {
    low = NULL;
    high = NULL;
    name ="NA";
}

Register16::Register16(Register8* high, Register8* low) {
    this->low = low;
    this->high = high;
    name =(*high).getName() + (*low).getName();
}

Register16::Register16(uint16_t data,std::string name) {
    this->data = data;
    this->name = name;
}

void Register16::setLow(Register8* r) {
    this->low = r;
}

void Register16::setHigh(Register8* r) {
    this->high = r;
}

Register8* Register16::getLow() {
    return low;
}

uint8_t Register16::getLowData() {
    return low->getData(); 
}

Register8* Register16::getHigh() {
    return high;
}

uint8_t Register16::getHighData() {
    return high->getData();
}

uint16_t Register16::getData() {
    if (this->name == "SP" || this->name == "PC") {
        return this->data;
    } else {
        return ((*low).getData() | ((*high).getData() << 8));
    }
}

uint16_t Register16::getData_() {
    return data;
}

std::string Register16::getName() {
    return name;    
}

void Register16::setData(uint16_t data) {
    if (this->name == "SP" || this->name == "PC") {
        this->data = data;
    } else {
        uint8_t l = data & 0x00ff;
        uint8_t h = (data & 0xff00) >> 8;
        (*low).setData(l);
        (*high).setData(h);
    }
}

void Register16::setData_(uint16_t data) {
    this->data = data;
}

void Register16::setName(std::string name) {
    this->name = name;
}

void Register16::inc(uint16_t n) {
    this-> data = this -> data + n;
}

void Register16::dec(uint16_t n) {
    this-> data = this -> data - n;
}
