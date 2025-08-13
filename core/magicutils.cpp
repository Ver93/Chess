#include "magicutils.h"

namespace MagicUtils {
    uint64_t generateSlidingMoves(int square, uint64_t blockerConfig, int pieceType){
        uint64_t moves      = 0ULL;
        int rank            = square / 8;
        int file            = square % 8;
        int start, end;

        switch (pieceType){
            case Const::PT_BISHOP: start = 4; end = 8; break;
            case Const::PT_ROOK: start = 0; end = 4; break;
            default: start = 0; end = 8; break;
        }

        for (int d = start; d < end; ++d){
            int dr = Const::SLIDING_MOVES[d][0];
            int df = Const::SLIDING_MOVES[d][1];

            int r = rank + dr;
            int f = file + df;

            while (r >= 0 && r < 8 && f >= 0 && f < 8) {

                int targetSquare = r * 8 + f;
                uint64_t sqMask = 1ULL << targetSquare;
                moves |= sqMask;

                if (blockerConfig & sqMask) break;

                r += dr;
                f += df;
            }
        }
        return moves;
    }

    uint64_t generateJumpingMoves(int square, int pieceType, bool isWhite){
        uint64_t sqMask = 1ULL << square;
        uint64_t moves = 0ULL;

        switch (pieceType) {
        case Const::PT_PAWN:
            if (isWhite) {
                moves |= (sqMask << 7) & Const::EXCLUDE_H_FILE;
                moves |= (sqMask << 9) & Const::EXCLUDE_A_FILE;
            } else {
                moves |= (sqMask >> 7) & Const::EXCLUDE_A_FILE;
                moves |= (sqMask >> 9) & Const::EXCLUDE_H_FILE;
            }
            break;

        case Const::PT_KNIGHT:
            moves |= (sqMask << 6) & Const::EXCLUDE_GH_FILE;
            moves |= (sqMask << 10) & Const::EXCLUDE_AB_FILE;
            moves |= (sqMask << 15) & Const::EXCLUDE_H_FILE;
            moves |= (sqMask << 17) & Const::EXCLUDE_A_FILE;

            moves |= (sqMask >> 6) & Const::EXCLUDE_AB_FILE;
            moves |= (sqMask >> 10) & Const::EXCLUDE_GH_FILE;
            moves |= (sqMask >> 15) & Const::EXCLUDE_A_FILE;
            moves |= (sqMask >> 17) & Const::EXCLUDE_H_FILE;
            break;

        case Const::PT_KING:
            if (sqMask & Const::EXCLUDE_H_FILE) moves |= sqMask << 1;
            if (sqMask & Const::EXCLUDE_A_FILE) moves |= sqMask >> 1;
            moves |= sqMask << 8;
            moves |= sqMask >> 8;
            if (sqMask & Const::EXCLUDE_H_FILE) {
                moves |= sqMask << 9;
                moves |= sqMask >> 7;
            }
            if (sqMask & Const::EXCLUDE_A_FILE) {
                moves |= sqMask << 7;
                moves |= sqMask >> 9;
            }
            break;
        }
        return moves;
    }

    bool testMagic(std::vector<uint64_t>& permutations, std::vector<uint64_t>& used, uint64_t magic, int relevantBits, int pt, int square, int arraySize) {
        for (uint64_t blocker : permutations) {
            uint64_t index = (blocker * magic) >> (64 - relevantBits);

            if (index >= arraySize) {
                return false;
            }

            uint64_t attackMask = generateSlidingMoves(square, blocker, pt);
            if (used[index] == 0ULL) {
                used[index] = attackMask;
            } else if (used[index] != attackMask) {
                return false;
            }
        }
        return true;
    }

        std::vector<uint64_t> calcTotalPermutations(uint64_t mask) {
        std::vector<uint64_t> permutations;
        int bitIndices[64], bitCount = 0;

        for (int i = 0; i < 64; ++i)
            if (mask & (1ULL << i))
                bitIndices[bitCount++] = i;

        int total = 1 << bitCount;
        permutations.reserve(total);

        for (int i = 0; i < total; ++i) {
            uint64_t blockers = 0;
            for (int j = 0; j < bitCount; ++j)
                if (i & (1 << j))
                    blockers |= (1ULL << bitIndices[j]);
            permutations.push_back(blockers);
        }

        return permutations;
    }

    uint64_t rand64() {
        return ((uint64_t)rand() & 0xFFFF) |
        (((uint64_t)rand() & 0xFFFF) << 16) |
        (((uint64_t)rand() & 0xFFFF) << 32) |
        (((uint64_t)rand() & 0xFFFF) << 48);
    }

    uint64_t randomMagic(){
        return rand64() & rand64() & rand64();
    }

    bool filterHeuristics(uint64_t magic, uint64_t mask, int shift){
        return (popcount64((magic * mask) & 0xFF00000000000000ULL) < shift);
    }

    bool fileExists(const std::string& path) {
        std::ifstream file(path);
        return file.good();
    }

    uint64_t findMagic(int square, int relevantBits, int pt, uint64_t& mask, std::vector<uint64_t> permutations) {
        const int tableSize = 1 << relevantBits;

        std::vector<uint64_t> used(tableSize, 0ULL);

        switch (pt){
        case Const::PT_BISHOP:
            for (int trial = 0; trial < 10000000; ++trial) {
                uint64_t magic = MagicUtils::randomMagic();
                if(MagicUtils::filterHeuristics(magic, mask, 6)) continue;
                used.assign(tableSize, 0ULL);
                if(testMagic(permutations, used, magic, relevantBits, pt, square, Const::MAX_BISHOP_ENTRIES)) return magic;
            }
            break;
        case Const::PT_ROOK:
            for (int trial = 0; trial < 10000000; ++trial) {
                if(relevantBits == 0){
                    return 0ULL;
                }
                uint64_t magic = MagicUtils::randomMagic();
                if(MagicUtils::filterHeuristics(magic, mask, 5)) continue;
                used.assign(tableSize, 0ULL);
                if(testMagic(permutations, used, magic, relevantBits, pt, square, Const::MAX_ROOK_ENTRIES)) return magic;
            }
            break;
        }

        std::cerr << "Failed to find magic for square " << square << "\n";
        return 0ULL;
    }

        void saveMagicData(std::unique_ptr<AttackData>& dataPtr, const std::string& saveString) {
        if (!dataPtr) {
            std::cerr << "MagicData pointer is null. Cannot save.\n";
            return;
        }

        std::ofstream out(saveString, std::ios::binary);
        if (!out) {
            std::cerr << "Failed to open file for writing: " << saveString << "\n";
            return;
        }

        const AttackData& data = *dataPtr;
        out.write(reinterpret_cast<const char*>(data.rookBlockerMasks), sizeof(data.rookBlockerMasks));
        out.write(reinterpret_cast<const char*>(data.rookBits), sizeof(data.rookBits));
        out.write(reinterpret_cast<const char*>(data.rookMagics), sizeof(data.rookMagics));
        out.write(reinterpret_cast<const char*>(data.rookShiftAmount), sizeof(data.rookShiftAmount));
        out.write(reinterpret_cast<const char*>(data.bishopBlockerMasks), sizeof(data.bishopBlockerMasks));
        out.write(reinterpret_cast<const char*>(data.bishopBits), sizeof(data.bishopBits));
        out.write(reinterpret_cast<const char*>(data.bishopMagics), sizeof(data.bishopMagics));
        out.write(reinterpret_cast<const char*>(data.bishopShiftAmount), sizeof(data.bishopShiftAmount));
        out.write(reinterpret_cast<const char*>(data.knightAttackTable), sizeof(data.knightAttackTable));
        out.write(reinterpret_cast<const char*>(data.kingAttackTable), sizeof(data.kingAttackTable));
        out.write(reinterpret_cast<const char*>(data.whitePawnAttackTable), sizeof(data.whitePawnAttackTable));
        out.write(reinterpret_cast<const char*>(data.blackPawnAttackTable), sizeof(data.blackPawnAttackTable));
        out.write(reinterpret_cast<const char*>(data.whitePawnMoveTable), sizeof(data.whitePawnMoveTable));
        out.write(reinterpret_cast<const char*>(data.blackPawnMoveTable), sizeof(data.blackPawnMoveTable));

        // Bishop attack table
        for (int i = 0; i < 64; ++i) {
            out.write(reinterpret_cast<const char*>(data.bishopAttackTable[i]),
                      sizeof(uint64_t) * Const::MAX_BISHOP_ENTRIES);
        }

        // Rook attack table
        for (int i = 0; i < 64; ++i) {
            out.write(reinterpret_cast<const char*>(data.rookAttackTable[i]),
                      sizeof(uint64_t) * Const::MAX_ROOK_ENTRIES);
        }

        out.close();
        std::cout << "Magic Tables Saved Successfully to " << saveString << std::endl;
    }

    std::unique_ptr<AttackData> loadMagicData(const std::string& loadString) {
        std::ifstream in(loadString, std::ios::binary);
        if (!in) {
            std::cerr << "Failed to open file for reading: " << loadString << "\n";
            return nullptr;
        }

        auto dataPtr = std::make_unique<AttackData>();
        AttackData& data = *dataPtr;
        in.read(reinterpret_cast<char*>(data.rookBlockerMasks), sizeof(data.rookBlockerMasks));
        in.read(reinterpret_cast<char*>(data.rookBits), sizeof(data.rookBits));
        in.read(reinterpret_cast<char*>(data.rookMagics), sizeof(data.rookMagics));
        in.read(reinterpret_cast<char*>(data.rookShiftAmount), sizeof(data.rookShiftAmount));
        in.read(reinterpret_cast<char*>(data.bishopBlockerMasks), sizeof(data.bishopBlockerMasks));
        in.read(reinterpret_cast<char*>(data.bishopBits), sizeof(data.bishopBits));
        in.read(reinterpret_cast<char*>(data.bishopMagics), sizeof(data.bishopMagics));
        in.read(reinterpret_cast<char*>(data.bishopShiftAmount), sizeof(data.bishopShiftAmount));
        in.read(reinterpret_cast<char*>(data.knightAttackTable), sizeof(data.knightAttackTable));
        in.read(reinterpret_cast<char*>(data.kingAttackTable), sizeof(data.kingAttackTable));
        in.read(reinterpret_cast<char*>(data.whitePawnAttackTable), sizeof(data.whitePawnAttackTable));
        in.read(reinterpret_cast<char*>(data.blackPawnAttackTable), sizeof(data.blackPawnAttackTable));
        in.read(reinterpret_cast<char*>(data.whitePawnMoveTable), sizeof(data.whitePawnMoveTable));
        in.read(reinterpret_cast<char*>(data.blackPawnMoveTable), sizeof(data.blackPawnMoveTable));

        for (int i = 0; i < 64; ++i) {
            in.read(reinterpret_cast<char*>(data.bishopAttackTable[i]),
                    sizeof(uint64_t) * Const::MAX_BISHOP_ENTRIES);
        }

        // Rook attack table
        for (int i = 0; i < 64; ++i) {
            in.read(reinterpret_cast<char*>(data.rookAttackTable[i]),
                    sizeof(uint64_t) * Const::MAX_ROOK_ENTRIES);
        }

        in.close();
        std::cout << "Magic Tables Loaded Successfully" << std::endl;
        return dataPtr;
    }
}