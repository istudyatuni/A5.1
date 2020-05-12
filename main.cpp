#include <iostream>
using namespace std;

typedef unsigned long word;
typedef unsigned long long int64;

class GsmCipher {
    word R1, R2, R3;//linear feedback shift registers
    word R1MASK = 0x7ffff;//19 bit
    word R2MASK = 0x3fffff;//22 bit
    word R3MASK = 0x7fffff;//23 bit
    word R1TAPS = 0x72000; //bits 18,17,16,13
    word R2TAPS = 0x300000; //bits 21,20
    word R3TAPS = 0x700080; //bits 22,21,20,7
    word R1MID = 0x100;//8 bit
    word R2MID = 0x400;//10 bit
    word R3MID = 0x400;//10 bit
    word R1OUT = 0x40000;//18-th bit
    word R2OUT = 0x200000;//21-th bit
    word R3OUT = 0x400000;//22-th bit
    int64 key;
    word iv;
    void initRegisters() {
        R1 = R2 = R3 = 0;
        int keybit, ivbit;
        for (int i = 0; i < 64; ++i) {
            clockall();
            keybit = (key >> i) & 1;//i-th bit of key
            R1 ^= keybit; R2 ^= keybit; R3 ^= keybit;
        }
        for (int i=0; i<22; i++) {
            clockall();
            ivbit = (iv >> i) & 1; //i-th bit of iv
            R1 ^= ivbit; R2 ^= ivbit; R3 ^= ivbit;
        }
        for (int i = 0; i < 100; ++i) {
            clock();
        }
    }
    int a = 313;
    int maj() {//функция большинства
        int a = (R1 >> 8) & 1,
            b = (R2 >> 10) & 1,
            c = (R3 >> 10) & 1;//10-th bit of R3
        return (a & b) | (a & c) | (b & c);
    }
    int parity(word a) {//замена xor нескольких бит
        int r = 0;
        for (int i = 0; i < 32; ++i) {
            r += (a & 1);
            a >>= 1;
        }
        //sum of bits modulo 2
        return r%2;
    }
    void clock() {
        int m = maj();
        if (((R1&R1MID) != 0) == m)
            clockone(R1, R1MASK, R1TAPS);
        if (((R2&R2MID) != 0) == m)
            clockone(R2, R2MASK, R2TAPS);
        if (((R3&R3MID) != 0) == m)
            clockone(R3, R3MASK, R3TAPS);
    }
    void clockone(word& reg, word mask, word tap) {
        word t = reg & tap;
        reg = (reg << 1) & mask;
        reg |= parity(t);
    }
    void clockall() {
        clockone(R1, R1MASK, R1TAPS);
        clockone(R2, R2MASK, R2TAPS);
        clockone(R3, R3MASK, R3TAPS);
    }
    int64 getbit() {
        return parity(R1&R1OUT)^parity(R2&R2OUT)^parity(R3&R3OUT);
    }
public:
    GsmCipher(int64 key, word iv):key(key), iv(iv){}
    void crypt(int64& text) {
        //maybe if encrypting big massive,
        //set initReg in constructor
        initRegisters();
        for (unsigned long i = 0; i < sizeof(text) * 8; ++i) {
            clock();
            //be careful: getbit returns just 64-bit word
            text ^= getbit() << i;
        }
    }
};

int main() {
    cout << hex;
    int64 key = 0x1223456789abcdef;
    word iv = 0x134;//initial value, frame
    GsmCipher a(key, iv);
    int64 test = 0x5238ef3a32b98cf2;
    cout << "plain text    = " << test << '\n';
    a.crypt(test);
    cout << "encrypted msg = " << test << '\n';
    a.crypt(test);
    cout << "decrypted msg = " << test << '\n';
    return 0;
}
