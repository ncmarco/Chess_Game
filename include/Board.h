//
// Created by coman on 8/10/2024.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <SFML/Audio.hpp>
using namespace sf;
#ifndef BOARD_H
#define BOARD_H

#define set_bit(bitboard, square) (bitboard |= (1ULL << square))    //bitboard - al piesei de sah , square - [0-63], pozitia pe tabla ; 1ULL << (square) creeaza un bit cu val 1 si il plaseaza pe pozitia square;  |=  -  il seteaza
#define get_bit(bitboard, square) (bitboard & (1ULL << square))     // & - verifica daca bitul de pe pozitia square este 1
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))   // ~() - creeaza un bit cu valoarea 0 la pozitia square ; &= - seteaza bitul la valoarea 0 in bitboard


struct Move {
    int originSq;
    int destinationSq;
};
int getSquareFromMousePosition(int mouseX, int mouseY);


// tabla de sah - interna si GUI
class Board {
private:
    int selectedSquare;
    int previousSquare;
    RenderWindow window;
    bool isWhiteTurn;
    // SoundBuffer moveSoundBuffer;
    // Sound moveSound;
    // SoundBuffer captureSoundBuffer;
    // Sound captureSound;
    Texture textures[12];
    Sprite pieces[12];
    void loadTextures();
    // void loadSounds();
    void drawBoard();
    void drawPieces();
    uint64_t whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueen, whiteKing,
             blackPawns, blackRooks, blackKnights, blackBishops, blackQueen, blackKing;
    uint64_t* getPieceTypeOnSquare(int square);
    void makeMove(uint64_t &bitboard, Move move);
    void capturePiece(int square);
    void drawSelectedSquare(int file, int rank);
public:
    Board();
    void run();
};


//void displayGraphicalBoard();
#endif //BOARD_H
