#include "magics.h"

namespace Magics {
    void initialize() {
        AttackGlobals::ad = std::make_unique<AttackData>();

        std::filesystem::path dataDir = std::filesystem::current_path() / "data";
        MagicUtils::ensureDataDirectoryExists(dataDir);

        std::string path = (dataDir / "Magics.dat").string();

        if (MagicUtils::fileExists(path)) {
            AttackGlobals::ad = MagicUtils::loadMagicData(path);
            return;
        }

        std::cout << "Magic initializing... " << std::endl;

        for (int square = 0; square < 64; square++) {
            auto& data = *AttackGlobals::ad;

            data.bishopBlockerMasks[square] = MagicUtils::generateSlidingMoves(square, 0ULL, Const::PT_BISHOP);
            data.rookBlockerMasks[square] = MagicUtils::generateSlidingMoves(square, 0ULL, Const::PT_ROOK);
            data.whitePawnAttackTable[square] = MagicUtils::generateJumpingMoves(square, Const::PT_PAWN, true);
            data.whitePawnMoveTable[square] = MagicUtils::generateJumpingMoves(square, Const::PT_PAWN, true);
            data.blackPawnAttackTable[square] = MagicUtils::generateJumpingMoves(square, Const::PT_PAWN, false);
            data.blackPawnMoveTable[square] = MagicUtils::generateJumpingMoves(square, Const::PT_PAWN, true);
            data.knightAttackTable[square] = MagicUtils::generateJumpingMoves(square, Const::PT_KNIGHT, false);
            data.kingAttackTable[square] = MagicUtils::generateJumpingMoves(square, Const::PT_KING, false);

            data.bishopBits[square] = popcount64(data.bishopBlockerMasks[square]);
            data.rookBits[square] = popcount64(data.rookBlockerMasks[square]);
            data.bishopShiftAmount[square] = 64 - data.bishopBits[square];
            data.rookShiftAmount[square] = 64 - data.rookBits[square];

            const auto b_permutations = MagicUtils::calcTotalPermutations(data.bishopBlockerMasks[square]);
            const auto r_permutations = MagicUtils::calcTotalPermutations(data.rookBlockerMasks[square]);

            data.bishopMagics[square] = MagicUtils::findMagic(square, data.bishopBits[square], Const::PT_BISHOP, data.bishopBlockerMasks[square], b_permutations);
            data.rookMagics[square] = MagicUtils::findMagic(square, data.rookBits[square], Const::PT_ROOK, data.rookBlockerMasks[square], r_permutations);

            for (uint64_t blockers : b_permutations) {
                uint64_t index = (blockers * data.bishopMagics[square]) >> (64 - data.bishopBits[square]);
                data.bishopAttackTable[square][index] = MagicUtils::generateSlidingMoves(square, blockers, Const::PT_BISHOP);
            }

            for (uint64_t blockers : r_permutations) {
                uint64_t index = (blockers * data.rookMagics[square]) >> (64 - data.rookBits[square]);
                data.rookAttackTable[square][index] = MagicUtils::generateSlidingMoves(square, blockers, Const::PT_ROOK);
            }

            std::cout << "Magic setup complete for square " << square << "\n";
        }

        MagicUtils::saveMagicData(AttackGlobals::ad, path);
        std::cout << "Magic initialization complete!" << std::endl;
    }
}