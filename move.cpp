#include "move.h"

std::ostream& operator<<(std::ostream& os, const Move& m) {
    int f = m.getFrom(), r = m.getTo();
    os << static_cast<char>(FILE64(f) + 'a') << ROW64(f) + 1;
    if (m.getCap() || m.isEnPas()) os << "X";
    os << static_cast<char>(FILE64(r) + 'a') << ROW64(r) + 1;

    if (m.getProm()) {
        switch (m.getProm()) {
            case 3: case 9:
                os << 'N'; break;
            case 4: case 10:
                os << 'B'; break;
            case 5: case 11:
                os << 'R'; break;
            case 6: case 12:
                os << 'Q'; break;
        }
    }

    return os;
}
