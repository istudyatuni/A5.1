#include <iostream>
using namespace std;

typedef unsigned long word;

class int64 {
    word bits[2];
public:
    int64(word b1, word b2):bits{b1, b2}{}
    word get(int i) {
        return bits[i];
    }
};

class Sequence {
public:
    Sequence();
    ~Sequence();
    
};

class GsmCipher {
    word R1, R2, R3;//linear shift registers
    int64 key;
    void initRegisters() {
        R1 = R2 = R3 = 0;
        
    }
    void xorFlow(word& text) {
        
    }
public:
    GsmCipher(int64 key):key(key) {
        
    }
};

int main() {
    cout << hex;
    int64 key(0xffeeddcc, 0xbbaa9988);
    return 0;
}
