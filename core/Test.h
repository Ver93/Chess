#pragma once

#include <future>
#include <vector>
#include <cstring>
#include <memory>
#include <iostream>
#include <mutex>

#include "State.h"
#include "MoveCounter.h"
#include "ThreatGen.h"
#include "MoveGen.h"
#include "Move.h"
#include "Undo.h"
#include "MoveExec.h"
#include "Const.h"
#include "Utils.h"

namespace Test {

    void countMoveType(Move& move, MoveCounter& counter) {
        switch (move.movingType) {
            case Const::MT_CAPTURE:               counter.capture++;   break;
            case Const::MT_ENPASSANT:             counter.enpassant++; counter.capture++; break;
            case Const::MT_CASTLE:                counter.castle++;    break;
            case Const::MT_PROMOTION:             counter.promotion++; break;
            case Const::MT_PROMOTION_CAPTURE:     counter.promotion++; counter.capture++; break;
        }
    }

    int perft(int depth, State& state, MoveCounter& counters, std::vector<Move>& path) {
        if (depth == 0) return 1;
        int nodes = 0;
        ThreatGen::updateThreats(state);
        std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
        Undo undo;

        for (auto& move : pseudo) {
            if (move.movingType == Const::MT_CASTLE) {
                int kingSquare = (state.turn == Const::PC_WHITE) ? Const::SQ_E1 : Const::SQ_E8;
                if (state.threatMap[state.turn ^ 1] & (1ULL << kingSquare)) continue;
                if (!MoveExec::isCastlingPathSafe(state, move.to, state.threatMap[state.turn ^ 1])) continue;
            }

            path.push_back(move);
            MoveExec::makeMove(state, move, undo);
            ThreatGen::updateThreats(state);

            if (MoveExec::isOpponentKingInCheck(state)) {
                path.pop_back();
                MoveExec::undoMove(state, undo);
                continue;
            }

            int subnodes = perft(depth - 1, state, counters, path);

            if (depth == 1 && subnodes > 0) {
                countMoveType(move, counters);
            }

            path.pop_back();
            MoveExec::undoMove(state, undo);
            nodes += subnodes;
        }
        return nodes;
    }

    struct PerftResult {
        long long nodes = 0;
        MoveCounter counter;
    };

    PerftResult perftTask(int depth, State& originalState, Move move) {
        State state = originalState;
        Undo undo;
        PerftResult result;
        std::vector<Move> path;

        if (move.movingType == Const::MT_CASTLE) {
            int kingSquare = (state.turn == Const::PC_WHITE) ? Const::SQ_E1 : Const::SQ_E8;
            if (state.threatMap[state.turn ^ 1] & (1ULL << kingSquare)) return result;
            if (!MoveExec::isCastlingPathSafe(state, move.to, state.threatMap[state.turn ^ 1])) return result;
        }

        path.push_back(move);
        MoveExec::makeMove(state, move, undo);
        ThreatGen::updateThreats(state);

        if (MoveExec::isOpponentKingInCheck(state)) {
            path.pop_back();
            MoveExec::undoMove(state, undo);
            return result;
        }

        result.nodes = perft(depth - 1, state, result.counter, path);

        if (depth == 1 && result.nodes > 0) {
            countMoveType(move, result.counter);
        }

        path.pop_back();
        MoveExec::undoMove(state, undo);
        return result;
    }

    void dividePerft(int depth, State& state) {
        std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
        std::vector<std::future<PerftResult>> futures;

        for (auto& move : pseudo) {
            futures.emplace_back(std::async(std::launch::async, perftTask, depth, std::ref(state), move));
        }

        long long totalNodes = 0;
        MoveCounter globalCounter;

        for (auto& future : futures) {
            PerftResult result = future.get();
            totalNodes += result.nodes;
            globalCounter += result.counter;
            Utils::print(result.nodes);
        }

        Utils::print(totalNodes);
        globalCounter.print();
    }
}