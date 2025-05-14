//
// Created by coman on 8/10/2024.
//

#include "Board.h"
#include <iostream>

Board::Board() : window(VideoMode(960,960), "Chess"), selectedSquare(-1), previousSquare(-1), isWhiteTurn(true) {
    //initializam bitboard-urile
    //white
    whitePawns = 0x000000000000FF00;
    whiteRooks = 0x0000000000000081;
    whiteBishops = 0x0000000000000042;
    whiteKnights = 0x0000000000000024;
    whiteQueen = 0x0000000000000010;
    whiteKing = 0x0000000000000008;

    //black
    blackKing = 0x0800000000000000;
    blackQueen = 0x1000000000000000;
    blackKnights = 0x2400000000000000;
    blackBishops = 0x4200000000000000;
    blackRooks = 0x8100000000000000;
    blackPawns = 0x00FF000000000000;

    //incarcam texturile (imaginile cu piese)
    loadTextures();
}
// void Board::loadSounds() {
//     if (!moveSoundBuffer.loadFromFile("sound/move-self.mp3")) {
//         std::cout << "Eroare la incarcarea sunetului!" << std::endl;
//         return;
//     }
//     moveSound.setBuffer(moveSoundBuffer);
//     if(!captureSoundBuffer.loadFromFile("sound/capture.mp3")) {
//         std::cout << "Eroare la incarcarea sunetului!" << std::endl;
//         return;
//     }
//     captureSound.setBuffer(captureSoundBuffer);
// }

void Board::loadTextures() {
    // incarcam texturile
    if (!textures[0].loadFromFile("images/white-pawn.png") ||
        !textures[1].loadFromFile("images/white-bishop.png") ||
        !textures[2].loadFromFile("images/white-knight.png") ||
        !textures[3].loadFromFile("images/white-rook.png") ||
        !textures[4].loadFromFile("images/white-queen.png") ||
        !textures[5].loadFromFile("images/white-king.png") ||
        !textures[6].loadFromFile("images/black-pawn.png") ||
        !textures[7].loadFromFile("images/black-bishop.png") ||
        !textures[8].loadFromFile("images/black-knight.png") ||
        !textures[9].loadFromFile("images/black-rook.png") ||
        !textures[10].loadFromFile("images/black-queen.png") ||
        !textures[11].loadFromFile("images/black-king.png"))
    {
        std::cerr << "Eroare la încărcarea texturilor!" << std::endl;
        return;
    }

    // cream sprite-urile pentru piese
    for (int i = 0; i < 12; i++) {
        textures[i].setSmooth(true);
        pieces[i].setTexture(textures[i]);
    }
}

void Board::drawBoard() {
    // Desenează tabla de șah
    for(int rank = 0; rank < 8; rank++) {
        for(int file = 0; file < 8; file++) {
            RectangleShape square(Vector2f(120.f, 120.f));
            square.setPosition(file * 120, rank * 120);
            if((file + rank) % 2 == 0)
                square.setFillColor(Color(173, 216, 230));
            else
                square.setFillColor(Color(41, 110, 133));
            int currentSquare = rank * 8 + file;
            if (63 - currentSquare == selectedSquare) {
                square.setFillColor(Color(67, 166, 198));
            }
            if (63 - currentSquare == previousSquare) {
                square.setFillColor(Color(103, 183, 209));
            }
            window.draw(square);
        }
    }
    drawPieces();
}

void Board::drawPieces() {
    // Desenează piesele pe tabla de șah folosind bitboard-uri
    for (int i = 0; i < 64; i++) {
        uint64_t bit = 1ULL << (63 - i);
        int file = i % 8;
        int rank = i / 8;
        if (whitePawns & bit) {
            pieces[0].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[0]);
        }
        else if (whiteKnights & bit) {
            pieces[1].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[1]);
        }
        else if (whiteBishops & bit) {
            pieces[2].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[2]);
        }
        else if (whiteRooks & bit) {
            pieces[3].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[3]);
        }
        else if (whiteQueen & bit) {
            pieces[4].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[4]);
        }
        else if (whiteKing & bit) {
            pieces[5].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[5]);
        }
        else if (blackPawns & bit) {
            pieces[6].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[6]);
        }
        else if (blackKnights & bit) {
            pieces[7].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[7]);
        }
        else if (blackBishops & bit) {
            pieces[8].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[8]);
        }
        else if (blackRooks & bit) {
            pieces[9].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[9]);
        }
        else if (blackQueen & bit) {
            pieces[10].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[10]);
        }
        else if (blackKing & bit) {
            pieces[11].setPosition(file * 120 + 10, rank * 120 + 10);
            window.draw(pieces[11]);
        }
    }
}
void Board::run() {
    while(window.isOpen()) {
        Event event;
        Move move;
        while(window.pollEvent(event)) {
            if(event.type == Event::Closed)
                window.close();       // inchide fereastra
            if(event.type == Event::MouseButtonPressed) {
                selectedSquare = -1;
                previousSquare = -1;
                int originSquare = getSquareFromMousePosition(event.mouseButton.x, event.mouseButton.y);     // calculeaza patratul pe care am dat click
                move.originSq = 63 - originSquare;    // il converteste la valoarea corect pt a manipula bitboard-urile
                selectedSquare = move.originSq;      // modifica variabila clasei pentru a putea desena patratul selectat
                int file = event.mouseButton.x / 120;
                int rank = event.mouseButton.y / 120;
                drawSelectedSquare(file, rank);
            }
            if(event.type == Event::MouseButtonReleased) {
                int destinationSquare = getSquareFromMousePosition(event.mouseButton.x, event.mouseButton.y);
                move.destinationSq = 63 - destinationSquare;
                int file = event.mouseButton.x / 120;
                int rank = event.mouseButton.y / 120;
                uint64_t* pieceOnSquare = getPieceTypeOnSquare(move.originSq);     // returneaza bitboardul piesei de pe patratul selectat
                if(pieceOnSquare != nullptr && move.originSq != move.destinationSq) {   // daca piesa exista, va face mutarea
                    makeMove(*pieceOnSquare, move);
                    previousSquare = selectedSquare;
                    selectedSquare = move.destinationSq;
                    drawSelectedSquare(file, rank);
                }
                else {
                    std::cout << "Patratul nu este ocupat de catre nicio piesa sau nu ati mutat piesa!" << std::endl;
                }
            }
        }
        window.clear();
        drawBoard(); // Desenează tabla
        window.display(); // Afișează totul pe ecran
    }
}


void Board::makeMove(uint64_t &bitboard, Move move) {
    if ((isWhiteTurn && (bitboard & (whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueen | whiteKing))) ||
    (!isWhiteTurn && (bitboard & (blackPawns | blackRooks | blackKnights | blackBishops | blackQueen | blackKing)))) {
        // moveSound.play();
        capturePiece(move.destinationSq);
        // schimbam pozitia bit-ului in bitboard
        pop_bit(bitboard, move.originSq);
        set_bit(bitboard, move.destinationSq);
        std::cout << "Move made from " << move.originSq << " to " << move.destinationSq << std::endl;
        isWhiteTurn = !isWhiteTurn;
    }
    else {
        std::cout << "Mutare invalida! Este randul jucatorului: " << (isWhiteTurn ? "ALB!" : "NEGRU!") << std::endl;
    }
    std::cout << "Bitboard: " << bitboard << std::endl;
}
int getSquareFromMousePosition(int mouseX, int mouseY) {
    // calculeaza indicii patratului pe care am dat click
    int file = mouseX / 120 ;
    int rank = mouseY / 120 ;
    std::cout << mouseX << " " << mouseY << std::endl << file << " " << rank << std::endl;
    return rank * 8 + file;
}

uint64_t* Board::getPieceTypeOnSquare(int square) {
    // returneaza bitboard-ul piesei de pe un anumit patrat
    if(get_bit(whitePawns, square))
        return &whitePawns;
    else if(get_bit(whiteBishops, square))
        return &whiteBishops;
    else if(get_bit(whiteKnights, square))
        return &whiteKnights;
    else if(get_bit(whiteRooks, square))
        return &whiteRooks;
    else if (get_bit(whiteQueen, square))
        return &whiteQueen;
    else if(get_bit(whiteKing, square))
        return &whiteKing;
    else if(get_bit(blackPawns, square))
        return &blackPawns;
    else if(get_bit(blackBishops, square))
        return &blackBishops;
    else if(get_bit(blackKnights, square))
        return &blackKnights;
    else if(get_bit(blackRooks, square))
        return &blackRooks;
    else if (get_bit(blackQueen, square))
        return &blackQueen;
    else if(get_bit(blackKing, square))
        return &blackKing;
    else
        return nullptr;
}

void Board::capturePiece(int square) {
    // verificam daca pe patratul de destinatie exista deja o piesa, iar daca aceasta exista, este capturata
    uint64_t* pieceOnSquare = getPieceTypeOnSquare(square);
    if (pieceOnSquare != nullptr) {
        pop_bit(*pieceOnSquare, square);
        // captureSound.play();
    }
}

void Board::drawSelectedSquare(int file, int rank) {
    RectangleShape coloredSquare(Vector2f(120.f, 120.f));
    coloredSquare.setPosition(file * 120, rank * 120);
    coloredSquare.setFillColor(Color(67, 166, 198));
    window.draw(coloredSquare);
}

