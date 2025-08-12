#pragma once

#include <future>
#include <vector>
#include <cstring>
#include <memory>
#include <iostream>
#include <mutex>

#include "state.h"
#include "movecounter.h"
#include "threatgen.h"
#include "movegen.h"
#include "moveexec.h"
#include "MoveVal.h"
#include "move.h"
#include "undo.h"
#include "const.h"
#include "utils.h"

using PerftOutput = std::pair<long long, MoveCounter>;

namespace Test {
    void countMoveType(const Move& move, MoveCounter& counter) {
        switch (move.movingType) {
            case Const::MT_CAPTURE:               counter.capture++;   break;
            case Const::MT_ENPASSANT:             counter.enpassant++; counter.capture++; break;
            case Const::MT_CASTLE:                counter.castle++;    break;
            case Const::MT_PROMOTION:             counter.promotion++; break;
            case Const::MT_PROMOTION_CAPTURE:     counter.promotion++; counter.capture++; break;
        }
    }

    void countCheckAndCheckmate(State& state, MoveCounter& counter){
            if (MoveVal::isKingInCheckBeforeMove(state)) {
            counter.checks++;
            std::vector<Move> reply = MoveGen::generatePseudoMoves(state);
            bool hasLegalReply = false;
            Undo undo;
            for (const auto& replyMove : reply) {
                MoveExec::makeMove(state, replyMove, undo);
                MoveExec::switchTurn(state);
                ThreatGen::updateThreats(state);
                if (!MoveVal::isKingInCheckAfterMove(state)) {
                    hasLegalReply = true;
                }
                MoveExec::undoMove(state, undo);
                if (hasLegalReply) break;
            }
            if (!hasLegalReply) {
                counter.checkMates++;
            }
        }
    }

    int perft(int depth, State& state, MoveCounter& counters) {
        if (depth == 0) return 1;
        int nodes = 0;
        std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
        Undo undo;

        for (const auto& move : pseudo) {
            if (move.movingType == Const::MT_CASTLE) {
                if (MoveVal::isKingInCheckBeforeMove(state)) continue;
                if (!MoveVal::isCastlingPathSafe(state, move.to, state.threatMap[state.turn ^ 1])) continue;
            }

            MoveExec::makeMove(state, move, undo);
            MoveExec::switchTurn(state);
            ThreatGen::updateThreats(state);

            if (MoveVal::isKingInCheckAfterMove(state)) {
                MoveExec::undoMove(state, undo);
                continue;
            }

            int subnodes = perft(depth - 1, state, counters);

            if (depth == 1 && subnodes > 0) {
                countMoveType(move, counters);
                countCheckAndCheckmate(state, counters);
            }

            MoveExec::undoMove(state, undo);
            nodes += subnodes;
        }
        return nodes;
    }

    PerftOutput perftTask(int depth, State& originalState, const Move& move) {
        State state = originalState;
        Undo undo;
        long long nodes = 0;
        MoveCounter counter;

        if (move.movingType == Const::MT_CASTLE) {
            if (MoveVal::isKingInCheckBeforeMove(state)) return {0, counter};
            if (!MoveVal::isCastlingPathSafe(state, move.to, state.threatMap[state.turn ^ 1])) return {0, counter};
        }

        MoveExec::makeMove(state, move, undo);
        MoveExec::switchTurn(state);
        ThreatGen::updateThreats(state);

        if (MoveVal::isKingInCheckAfterMove(state)) {
            MoveExec::undoMove(state, undo);
            return {0, counter};
        }

        nodes = perft(depth - 1, state, counter);

        if (depth == 1 && nodes > 0) {
            countMoveType(move, counter);
            countCheckAndCheckmate(state, counter);
        }

        MoveExec::undoMove(state, undo);
        return {nodes, counter};
    }

    void dividePerft(int depth, State& state) {
        std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
        std::vector<std::future<PerftOutput>> futures;

        for (const auto& move : pseudo) {
                futures.emplace_back(std::async(std::launch::async, perftTask, depth, std::ref(state), move));
            }

            long long totalNodes = 0;
            MoveCounter globalCounter;

            for (auto& future : futures) {
                auto [nodes, counter] = future.get();
                totalNodes += nodes;
                globalCounter += counter;
                Utils::print(nodes);
            }

            Utils::print("Nodes Searched: ", totalNodes);
            globalCounter.print();

    }
}