#include "bitboard.hpp"
#include "show.hpp"
#include "bbfunctions.hpp"
#include "fen_parser.hpp"
#include "random.hpp"
#include "bitboard_fen.hpp"
#include "lookup.hpp"
#include "piece_attacks.hpp"
#include "move_gen.hpp"
#include "hiResTime.hpp"
#include "init.hpp"
#include "bitboard_zobrist.hpp"
#include "tt.hpp"
#include "perft.hpp"
#include "evaluation.hpp"
#include "opening_book_creator.hpp"

void RunPerft(const std::string& fen, int depth, int iters, ui64 value)
{
    Show<ShowTypes::Console>::Op(fen);
    Bitboard& b = BitboardFromFen(fen);
    b.zobrists.push_back( ZobristFromBitboard(b) );
    aks::time::HighResStopWatch w;
    w.startTimer();
    ui64 sum=0;
    w.startTimer();
    for(int i=0; i<iters; ++i)
    {
        PerftHelper ph;
        sum += Perft(b,depth,0, ph);
        //std::cout <<"ph.mates             "<< ph.mates             << std::endl;
        //std::cout <<"ph.captures          "<< ph.captures          << std::endl;
        //std::cout <<"ph.count             "<< ph.count             << std::endl;
        //std::cout <<"ph.ep                "<< ph.ep                << std::endl;
        //std::cout <<"ph.stalemates        "<< ph.stalemates        << std::endl;
        //std::cout <<"ph.promotions        "<< ph.promotions        << std::endl;
        //std::cout <<"ph.castle_kingsides  "<< ph.castle_kingsides  << std::endl;
        //std::cout <<"ph.castle_queensides "<< ph.castle_queensides << std::endl;
    }
    w.stopTimer();
    std::string dpt = to_string(depth);
    std::string num = to_string(sum/iters);
    std::string avg = to_string(sum/w.getElapsedTime());
    Show<ShowTypes::Console>::Op(std::string("Depth: ")+dpt);
    std::cout << sum << std::endl;;
    std::cout << value << std::endl;
    std::cout << (sum == value) << std::endl;
    Show<ShowTypes::Console>::Op(w.getElapsedTime()/iters);
    Show<ShowTypes::Console>::Op(avg);

    if((sum/iters) != value)
        Show<ShowTypes::Console>::Op("XXXXXXXXXXXXXXXXXXXXXXXXX");
    Show<ShowTypes::Console>::Op("----------------------");

}

void RunZobTest(const std::string& fen, int iters)
{
    Bitboard& b = BitboardFromFen(fen);
    ui64 zob_original = ZobristFromBitboard(b);
    b.zobrists.push_back(zob_original);
    int n_moves;
    bool go_on = true;
    for(int i=0; i<iters; ++i)
    {
        if(go_on)
        {
            GeneratePseudoLegalMoves(b, b.moves_arr[i]);
            n_moves = b.moves_arr[i].size();
            for(int j=0; j<10000; ++j)
            {
                int n = (n_moves-1)%Random<ui64>();
                move m = b.moves_arr[i][n];
                if(b.MakeMove(m))
                {
                    ui64 zob_now = ZobristFromBitboard(b);
                    b.zobrists.push_back(b.zobrists.back());
                    UpdateZobristFromMove(b.zobrists.back(), m, b);
                    if((zob_now != b.zobrists.back()))
                        Show<ShowTypes::Console>::Op( "No" );
                    break;
                }
                else
                {
                    go_on = false;
                    break;
                }
            }
        }
    }
}









int NegaMax(Bitboard& b, int depth, int current_depth, ui64& nodesSeen)
{
    int best = Constants::NEG_INF;
    if(depth <= 0) { ++nodesSeen; return evaluate(b); }

    GeneratePseudoLegalMoves(b, b.moves_arr[current_depth]);
    int n_moves = b.moves_arr[current_depth].size();

    for (int i = 0; i < n_moves; i++) 
    {
        move& m =b.moves_arr[current_depth][i];
        if(b.MakeMove(m))
        {
            b.zobrists.push_back(b.zobrists.back());
            UpdateZobristFromMove(b.zobrists.back(), m, b);
            int val = -NegaMax(b, depth-1, current_depth+1, nodesSeen);
            if( val > best )
                best = val;
            b.zobrists.pop_back();
        }
        b.UnmakeMove();
    }
    return best;
}
int GetNegaMaxScore(Bitboard& b, int depth, ui64& nodesSeen)
{
    //if(b.IsWhitesTurn())
    //  return NegaMax(b,depth,0);
    //else
    //  return -NegaMax(b,depth,0);
    int val = NegaMax(b,depth,0, nodesSeen);
    return val;
}

struct PV
{
    PV():cmove(0){}
    int cmove;                           // Number of moves in the line.
    move argmove[Constants::max_depth];  // The line.
    void Show() const
    {
        for(int i=0; i<cmove; ++i)
        {
            std::cout << Sq::SSq[argmove[i].from] << Sq::SSq[argmove[i].to] << " ";
        }
    }
};


int Quies(Bitboard& b, int depth, int alpha, int beta, int current_depth, ui64& nodesSeen)
{
    if(depth <= 0)
    {
        ++nodesSeen;
        return evaluate(b);
    }

    int stand_pat = evaluate(b);
    if( stand_pat >= beta )
    {
        ++nodesSeen;
        return beta;
    }
    if( alpha < stand_pat )
    {
        ++nodesSeen;
        alpha = stand_pat;
    }


    int val = 0;
    bool hasLegalMoves = false;

    GeneratePseudoLegalCaptureMoves(b, b.captures_arr[current_depth]);
    int n_moves = b.captures_arr[current_depth].size();

    for (int i = 0; i < n_moves; i++) 
    {
        move& m =b.captures_arr[current_depth][i];
        if(m.type == MoveType::capture || m.special == MoveType::promotion)
        {
            bool isMoveLegal = b.MakeMove(m);
            if(isMoveLegal)
            {
                hasLegalMoves = true;
                b.zobrists.push_back(b.zobrists.back());
                UpdateZobristFromMove(b.zobrists.back(), m, b);
                val = -Quies(b, depth-1, -beta, -alpha, current_depth+1, nodesSeen);
                if( val >= beta )
                {
                    b.zobrists.pop_back();
                    b.UnmakeMove();
                    return beta;
                }
                if( val > alpha )
                {
                    alpha = val;
                }
                b.zobrists.pop_back();
            }
            b.UnmakeMove();
        }
    }

    return alpha;
}

static TT gTT;
static OpeningBook::OpeningBookLoader* gBook;

int AlphaBeta(Bitboard& b, int depth, int alpha, int beta, int current_depth, ui64& nodesSeen, PV* pv, PV* prev_PV)
{
    BestMoveType::EBestMoveType hashType = BestMoveType::alpha;
    int hval = 0;
    move hash_move;
    TTProveReturnType::ETTProveReturnType probeResult = gTT.ProbeHash(depth, alpha, beta, b, hval, hash_move);
    switch(probeResult)
    {
    case TTProveReturnType::miss:
        break;
    case TTProveReturnType::value:
        //Show<ShowTypes::Console>::Op(hval);
        return hval;
        break;
    case TTProveReturnType::best_move:
        int temp=0;
        break;
    }

    PV pv_local;
    if(depth <= 0)
    {
        pv->cmove = 0;
        //++nodesSeen; 
        //int eval = evaluate(b);
        //if(eval > Constants::MATE_VALUE || eval < -Constants::MATE_VALUE)
        //{
        //    //Show<ShowTypes::Console>::Op("depth0");
        //    //Show<ShowTypes::Console>::Op(eval);
        //}
        int eval = Quies(b, 20, alpha, beta, 0, nodesSeen); 
        gTT.RecordHash(depth, eval, BestMoveType::exact, b);
        return eval;
    }

    int val = 0;
    bool hasLegalMoves = false;

    //trying all other moves.
    GeneratePseudoLegalMoves(b, b.moves_arr[current_depth]);

    //Trying hash move
    if(probeResult == TTProveReturnType::best_move)
    {
        b.moves_arr[current_depth].push_back(hash_move);
        //bool isMoveLegal = b.MakeMove(m);
        //if(isMoveLegal)
        //{
        //    hasLegalMoves = true;
        //    b.zobrists.push_back(b.zobrists.back());
        //    UpdateZobristFromMove(b.zobrists.back(), m, b);
        //    val = -AlphaBeta(b, depth-1, -beta, -alpha, current_depth+1, nodesSeen, &pv_local, 0);
        //    if( val >= beta )
        //    {
        //      gTT.RecordHash(depth, beta, BestMoveType::beta, b);
        //      b.zobrists.pop_back();
        //      b.UnmakeMove();
        //      return beta;
        //    }
        //    if( val > alpha )
        //    {
        //      hashType = BestMoveType::exact;
        //      alpha = val;
        //      pv->argmove[0] = m;
        //      memcpy(pv->argmove + 1, pv_local.argmove, pv_local.cmove * sizeof(move));
        //      pv->cmove = pv_local.cmove + 1;
        //    }
        //    b.zobrists.pop_back();
        //}
        //b.UnmakeMove();
    }

    //trying prev pv move
    if(prev_PV && prev_PV->cmove > current_depth)
    {
        move& m = prev_PV->argmove[current_depth];
        bool isMoveLegal = b.MakeMove(m);
        if(isMoveLegal)
        {
            hasLegalMoves = true;
            b.zobrists.push_back(b.zobrists.back());
            UpdateZobristFromMove(b.zobrists.back(), m, b);
            val = -AlphaBeta(b, depth-1, -beta, -alpha, current_depth+1, nodesSeen, &pv_local, prev_PV);
            if( val >= beta )
            {
                //Show<ShowTypes::Console>::Op("pv_beta");
                //Show<ShowTypes::Console>::Op(beta);
                gTT.RecordHash(depth, beta, BestMoveType::beta, b);
                b.zobrists.pop_back();
                b.UnmakeMove();
                return beta;
            }
            if( val > alpha )
            {
                hashType = BestMoveType::exact;
                alpha = val;
                pv->argmove[0] = m;
                memcpy(pv->argmove + 1, pv_local.argmove, pv_local.cmove * sizeof(move));
                pv->cmove = pv_local.cmove + 1;
            }
            b.zobrists.pop_back();
        }
        b.UnmakeMove();
    }

    ////trying all other moves.
    //GeneratePseudoLegalMoves(b, b.moves_arr[current_depth]);
    int n_moves = b.moves_arr[current_depth].size();

    for (int i = n_moves-1; i >= 0; --i) 
    {
        move& m =b.moves_arr[current_depth][i];
        bool isMoveLegal = b.MakeMove(m);
        if(isMoveLegal)
        {
            hasLegalMoves = true;
            b.zobrists.push_back(b.zobrists.back());
            UpdateZobristFromMove(b.zobrists.back(), m, b);
            val = -AlphaBeta(b, depth-1, -beta, -alpha, current_depth+1, nodesSeen, &pv_local, 0);
            if( val >= beta )
            {
                //Show<ShowTypes::Console>::Op("gen_beta");
                //Show<ShowTypes::Console>::Op(beta);
                gTT.RecordHash(depth, beta, BestMoveType::beta, b);
                b.zobrists.pop_back();
                b.UnmakeMove();
                return beta;
            }
            if( val > alpha )
            {
                hashType = BestMoveType::exact;
                alpha = val;
                pv->argmove[0] = m;
                memcpy(pv->argmove + 1, pv_local.argmove, pv_local.cmove * sizeof(move));
                pv->cmove = pv_local.cmove + 1;
            }
            b.zobrists.pop_back();
        }
        b.UnmakeMove();
    }

    //Show<ShowTypes::Console>::Op("gen_alpha");
    //Show<ShowTypes::Console>::Op(alpha);



    if(!hasLegalMoves)
    {
        if(b.IsKingInCheck())
        {
            int mating_value = Constants::MATE_VALUE - current_depth;
            if (mating_value < beta) 
            {
                beta = mating_value;
                if (alpha >= mating_value) 
                {
                    return mating_value;
                }
            }
            mating_value = -Constants::MATE_VALUE + current_depth;

            if (mating_value > alpha) 
            {
                alpha = mating_value;
                if (beta <= mating_value) 
                {
                    return mating_value;
                }
            }
        }
        else
        {
            return Constants::DRAW_VALUE;
        }
    }
    else
    {
        gTT.RecordHash(depth, alpha, hashType, b);
    }

    return alpha;
}
int GetAlphaBetaScore(Bitboard& b, int depth, ui64& nodesSeen, PV& pv, PV& prev_PV)
{
    //if(b.IsWhitesTurn())
    //  return NegaMax(b,depth,0);
    //else
    //  return -NegaMax(b,depth,0);
    int val = AlphaBeta(b,depth,Constants::NEG_INF,Constants::POS_INF,0,nodesSeen, &pv, &prev_PV);
    return val;
}

void CompareMiniMax_AlphaBeta(const std::string& fen, int depth)
{
    ui64 nodesSeen = 0;
    ui64 nodesSeenAB = 0;
    double mmTime = 0;
    double abTime = 0;
    aks::time::HighResStopWatch w;
    w.startTimer();
    //{
    //  Bitboard& b = BitboardFromFen(fen);
    //  b.zobrists.push_back( ZobristFromBitboard(b) );
    //  Show<ShowTypes::Console>::Op(GetNegaMaxScore(b, depth, nodesSeen));
    //}
    w.stopTimer();
    mmTime = w.getElapsedTime();
    w.startTimer();
    {
        PV pv;
        PV prev_pv;
        Bitboard& b = BitboardFromFen(fen);
        b.zobrists.push_back( ZobristFromBitboard(b) );
        int val = GetAlphaBetaScore(b, depth, nodesSeenAB, pv, prev_pv);
        Show<ShowTypes::Console>::Op(b);
        for(int i=0; i<pv.cmove; ++i)
        {
            //Show<ShowTypes::Console>::Op("----------");
            b.MakeMove(pv.argmove[i]);
            Show<ShowTypes::Console>::Op(b);
            Show<ShowTypes::Console>::Op(FenFromBitboard(b));
        }
        Show<ShowTypes::Console>::Op(val * ( b.IsWhitesTurn()?1:1));
    }
    w.stopTimer();
    abTime = w.getElapsedTime();
    Show<ShowTypes::Console>::Op((long)nodesSeen);
    Show<ShowTypes::Console>::Op((long)nodesSeenAB);
    Show<ShowTypes::Console>::Op((double)nodesSeenAB/nodesSeen);
    Show<ShowTypes::Console>::Op(mmTime);
    Show<ShowTypes::Console>::Op(abTime);
    Show<ShowTypes::Console>::Op(abTime/mmTime);
    Show<ShowTypes::Console>::Op(nodesSeen/mmTime);
    Show<ShowTypes::Console>::Op(nodesSeenAB/abTime);
    Show<ShowTypes::Console>::Op("------------------");
}

int absVal(int i){ return i<0?-i:i; }

void PlayGame(const std::string& fen)
{
    const std::string filename("gamefile_67.txt"); 
    Bitboard& b = BitboardFromFen(fen);
    b.zobrists.push_back( ZobristFromBitboard(b) );
    Show<ShowTypes::Console>::Op(b);
    Show<ShowTypes::File>::Op(filename, b);
    bool goOn = true;
    for(int n=0; n<300; ++n)
    {
        bool right = false;
        while(!right)
        {
            bool found = false;
            move mp;
            std::string s, sf, st;
            std::cout << "Input Move: " ;
            std::cin >> s;
            sf.clear();
            st.clear();
            sf += s[0];
            sf += s[1];
            st += s[2];
            st += s[3];

            Sq::ESq f,t;

            for(unsigned i=0; i<64; ++i)
            {
                if(sf == Sq::SSq[i])
                {
                    f = (Sq::ESq)i;
                }
                if(st == Sq::SSq[i])
                {
                    t = (Sq::ESq)i;
                }
            }

            type_array<move, 192> mvs;
            GeneratePseudoLegalMoves(b, mvs);

            if(s == std::string("o-o"))
            {
                for(int i=0; i<mvs.size(); ++i)
                {
                    if(mvs[i].special == MoveType::castle_kingside)
                    {
                        mp = mvs[i];
                        found = true;
                        break;
                    }
                }
            }
            else if( s == std::string("ooo"))
            {
                for(int i=0; i<mvs.size(); ++i)
                {
                    if(mvs[i].special == MoveType::castle_queenside)
                    {
                        mp = mvs[i];
                        found = true;
                        break;
                    }
                }
            }
            else if( s.length() > 4 )
            {
                PieceType::EPieceType p = PieceType::wqueens;
                switch(s[4])
                {
                case 'q':
                    p = PieceType::wqueens;
                    break;
                case 'r':
                    p = PieceType::wrooks;
                    break;
                case 'b':
                    p = PieceType::wbishops;
                    break;
                case 'n':
                    p = PieceType::wknights;
                    break;
                }
                for(int i=0; i<mvs.size(); ++i)
                {
                    if((mvs[i].from == f) && (mvs[i].to == t))
                    {
                        if(p == mvs[i].promoted)
                        {
                            mp = mvs[i];
                            found = true;
                            break;
                        }
                    }
                }                
            }
            else
            {
                for(int i=0; i<mvs.size(); ++i)
                {
                    if((mvs[i].from == f) && (mvs[i].to == t))
                    {
                        mp = mvs[i];
                        found = true;
                        break;
                    }
                }  
            }

            if(found)
            {
                if(b.MakeMove(mp))
                {
                   b.zobrists.push_back(b.zobrists.back());
                   UpdateZobristFromMove(b.zobrists.back(), mp, b);
                   Show<ShowTypes::Console>::Op(b);
                   Show<ShowTypes::File>::Op(filename, b);
                   right = true;
                }
                else
                {
                    std::cout << "Invalid move" << std::endl;
                    b.UnmakeMove();
                }
            }

        }
        if(goOn)
        {
            aks::time::HighResStopWatch w;
            int val = 0;
            int depth = 0;
            move bestMove;
            double totalTime = 0;
            double max_time = 2.0;//((n%2)==0) ? 5.0 : 0.25;
            type_array<PV, Constants::max_depth>* pvs = new type_array<PV, Constants::max_depth>();
            pvs->push_back();
            int alpha = Constants::NEG_INF;
            int beta = Constants::POS_INF;
            const int AspirationWindow = 50;
            bool forceCont = false;
            move opening_book_move;
            if(gBook->Query(b.zobrists.back(), opening_book_move))
            {
                bestMove = opening_book_move;
            }
            else
            {
                while((forceCont)||(totalTime < max_time && ((depth + absVal(val)) < Constants::MATE_VALUE)))
                {
                    forceCont = false;
                    PV& prev_pv = (*pvs)[depth];

                    ++depth;
                    pvs->push_back();
                    PV& pv = (*pvs)[depth];

                    ui64 nodesSeen = 0;
                    w.startTimer();
                    val =  AlphaBeta(b,depth,alpha,beta,0,nodesSeen, &pv, &prev_pv);
                    //val = GetAlphaBetaScore(b, depth, nodesSeen, pv, prev_pv);  
                    w.stopTimer();
                    totalTime += w.getElapsedTime();
                    if(max_time - totalTime < w.getElapsedTime())
                    {
                        totalTime = max_time;
                    }

                    if( absVal(val) == Constants::MATE_VALUE )
                    {
                        std::cout << (b.IsWhitesTurn() ? "White Mated" : "Black Mated") << std::endl;
                        goOn = false;
                        break;
                    }

                    std::cout << (b.IsWhitesTurn()? val:-val) << "\t" << nodesSeen << "\t\t" << w.getElapsedTime() << "\t\t";
                    pv.Show();
                    std::cout << std::endl;
                    bestMove = pv.argmove[0];

                    if ((val <= alpha) || (val >= beta)) 
                    {
                        Show<ShowTypes::Console>::Op("resetting");
                        alpha = Constants::NEG_INF;
                        beta =  Constants::POS_INF;
                        pv.cmove = 0;
                        --depth;
                        forceCont = true;
                        continue;
                    }
                    alpha = val - AspirationWindow;
                    beta  = val + AspirationWindow;
                }
            }
            delete pvs;

            if(goOn)
            {
                b.MakeMove(bestMove);
                b.zobrists.push_back(b.zobrists.back());
                UpdateZobristFromMove(b.zobrists.back(), bestMove, b);
                Show<ShowTypes::Console>::Op(b);
                Show<ShowTypes::File>::Op(filename, b);
                for(unsigned int i=0; i<b.moves.size(); ++i)
                {
                    std::cout << Sq::SSq[b.moves[i].from] << Sq::SSq[b.moves[i].to] << " ";
                }
                std::cout << std::endl;
            }
        }

        /*bool right = false;
        while(!right)
        {
            bool found = false;
            move mp;
            std::string s, sf, st;
            std::cout << "Input Move: " ;
            std::cin >> s;
            sf.clear();
            st.clear();
            sf += s[0];
            sf += s[1];
            st += s[2];
            st += s[3];

            Sq::ESq f,t;

            for(unsigned i=0; i<64; ++i)
            {
                if(sf == Sq::SSq[i])
                {
                    f = (Sq::ESq)i;
                }
                if(st == Sq::SSq[i])
                {
                    t = (Sq::ESq)i;
                }
            }

            type_array<move, 192> mvs;
            GeneratePseudoLegalMoves(b, mvs);

            if(s == std::string("o-o"))
            {
                for(int i=0; i<mvs.size(); ++i)
                {
                    if(mvs[i].special == MoveType::castle_kingside)
                    {
                        mp = mvs[i];
                        found = true;
                        break;
                    }
                }
            }
            else if( s == std::string("ooo"))
            {
                for(int i=0; i<mvs.size(); ++i)
                {
                    if(mvs[i].special == MoveType::castle_queenside)
                    {
                        mp = mvs[i];
                        found = true;
                        break;
                    }
                }
            }
            else if( s.length() > 4 )
            {
                PieceType::EPieceType p = PieceType::wqueens;
                switch(s[4])
                {
                case 'q':
                    p = PieceType::bqueens;
                    break;
                case 'r':
                    p = PieceType::brooks;
                    break;
                case 'b':
                    p = PieceType::bbishops;
                    break;
                case 'n':
                    p = PieceType::bknights;
                    break;
                }
                for(int i=0; i<mvs.size(); ++i)
                {
                    if((mvs[i].from == f) && (mvs[i].to == t))
                    {
                        if(p == mvs[i].promoted)
                        {
                            mp = mvs[i];
                            found = true;
                            break;
                        }
                    }
                }                
            }
            else
            {
                for(int i=0; i<mvs.size(); ++i)
                {
                    if((mvs[i].from == f) && (mvs[i].to == t))
                    {
                        mp = mvs[i];
                        found = true;
                        break;
                    }
                }  
            }

            if(found)
            {
                if(b.MakeMove(mp))
                {
                    b.zobrists.push_back(b.zobrists.back());
                    UpdateZobristFromMove(b.zobrists.back(), mp, b);
                    Show<ShowTypes::Console>::Op(b);
                    Show<ShowTypes::File>::Op(filename, b);
                    right = true;
                }
                else
                {
                    std::cout << "Invalid move" << std::endl;
                    b.UnmakeMove();
                }
            }

        }*/
    }

}

int main()
{
    size_t tablesizeMB = 1024;//2048;
    gTT.CreateTable(tablesizeMB*1024);
    Show<ShowTypes::Console>::Op((unsigned)sizeof(Bitboard));
    Show<ShowTypes::Console>::Op((unsigned)sizeof(move));
    Show<ShowTypes::Console>::Op((unsigned)sizeof(TT_E));
    Show<ShowTypes::Console>::Op((unsigned)sizeof(move_packed));
    //Show<ShowTypes::Console>::Op((unsigned)sizeof(TT_Entry));
    std::cout << ((gTT.size)) << std::endl;

    char a;
    //std::cin >> a; 

    init_all();

    bool RunPerfts = false;

    if(1)
    {
        const std::string book_file("D:/CODE/YACE/x64/Release/Book_2700PlusBoth_With_Fen.txt");
        gBook = new OpeningBook::OpeningBookLoader(book_file);

        gBook->Load();

        //delete gBook;
    }

    if(0)
    {
        const std::string fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        const std::string dbFile("C:/Users/Aks/Documents/Visual Studio 2010/Projects/moves_file_cleaner/Release/2700PlusBoth_cleaned.txt");
        const std::string outFile("book.txt");
        OpeningBook::OpeningBookCreator obc(fen, dbFile, outFile); 
        aks::time::HighResStopWatch w;
        w.startTimer();
        obc.Create();
        obc.WriteBookToFile("02_Book_2700PlusBoth_With_Fen.txt");
        w.stopTimer();
        Show<ShowTypes::Console>::Op(to_string(w.getElapsedTime()));
    }

    if(1)
    {
        //const std::string fen("k7/8/8/8/8/8/4R3/3K4 w - - 0 1");
        const std::string fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        //const std::string fen("r1q1kbr1/2BR2pb/2p1P2p/p4n1P/1p1Q2P1/1B6/PPP3K1/3R4 w - - 0 32");
        //const std::string fen("r2k3r/2n1q1b1/2Qp3p/8/p7/2PPN3/PP2PPPP/R3KB1R w KQ - 0 21");
        //const std::string fen("r1r2k2/6bQ/p2pp1p1/1pqP2P1/8/8/PPn5/1KBR3R b - - 0 1");
        //const std::string fen("8/4p3/7R/n7/rp6/kp5Q/8/1K6 w - - 0 1");
        //const std::string fen("8/k4ppp/8/K4PPP/8/8/8/8 w - - 0 1");
        // const std::string fen("8/8/8/8/ppp4k/8/PPP4K/8 b - - 0 1");
        //const std::string fen("1br1r3/3B4/5Bb1/3k3p/1PN5/1P1P1K2/3R2N1/7Q w - - 0 1");
        // const std::string fen("r1bqkb1r/ppp2pp1/2np3p/1B6/4n3/5N2/PPPP1PPP/RNBQ1RK1 w kq – 0 1");
        //const std::string fen("5B2/6p1/8/6pp/7k/5P2/q5PK/8 w - - 0 1");
        // const std::string fen("4r3/7q/nb2prRp/pk1p3P/3P4/P7/1P2N1P1/1K1B1N2 w - - 0 1");
        //const std::string fen("2r3k1/1b2p1b1/p5p1/1p2P1N1/5PQ1/1P6/q1P4P/2KR4 w - - 0 1");
        //const std::string fen("5K2/8/3p2k1/3P1R2/2P2p1P/2B2P2/8/8 w - - 0 1");
        //const std::string fen("3R4/1p2rppk/p1n4p/P7/2PR4/6KP/6P1/8 w - - 0 1");
        //const std::string fen("r1bqr1k1/1p3pp1/1pp4p/8/2P3n1/1Q1p4/PP1P1PPP/R1B1NK1R b - - 1 19");
        PlayGame(fen);
        //RunZobTest(fen, 100);
        //CompareMiniMax_AlphaBeta(fen, 1);
        //CompareMiniMax_AlphaBeta(fen, 2);
        //CompareMiniMax_AlphaBeta(fen, 3);
        //CompareMiniMax_AlphaBeta(fen, 4);
        //CompareMiniMax_AlphaBeta(fen, 5);
        //CompareMiniMax_AlphaBeta(fen, 6);
        //Bitboard& b = BitboardFromFen(fen);
        //Show<ShowTypes::Console>::Op(evaluate(b));
        //RunZobTest(fen, 100);
        //RunPerft(fen,  1, 1, 20);
        //RunPerft(fen,  2, 1, 400);
        //RunPerft(fen,  3, 1, 8902);
        //RunPerft(fen,  4, 1, 197281);
        //RunPerft(fen,  5, 1, 4865609);
        // RunPerft(fen,  6, 1, 119060324);
        // RunPerft(fen,  7, 1, 3195901860);
        // RunPerft(fen,  8, 1, 84998978956);
        // RunPerft(fen,  9, 1, 2439530234167);
        // RunPerft(fen, 10, 1, 69352859712417);
    }
    if(0)
    {
        const std::string fen("2k5/8/8/8/8/8/8/4R3/4K3 w - - 0 1");
        //const std::string fen("8/k7/2K5/1Q6/8/8/8/8 w - - 0 5");
        //const std::string fen("8/kQ6/2K5/8/8/8/8/8 b - - 0 5");
        // const std::string fen("1br1r3/3B4/5Bb1/3k3p/1PN5/1P1P1K2/3R2N1/7Q w - - 0 1");

        //const std::string fen("3brrb1/2N4B/8/2p4Q/2p2k2/5P2/4P1KR/2N2RB1 w - - 0 1");
        //const std::string fen("8/5p1k/6pp/3p4/Rpp2n1P/5P2/PbP2r1P/3K4 w - - 1 33");
        //const std::string fen("4k3/5R2/4PpN1/8/8/4pPn1/5r2/4K3 b - - 0 1");
        //const std::string fen("6k1/1p3Rpp/p1pN4/2P1n3/1PB5/7P/1q4PK/8 w - - 0 1");
        //const std::string fen("5K2/8/3p2k1/3P1R2/2P2p1P/2B2P2/8/8 w - - 0 1");
        //const std::string fen("4r3/7q/nb2prRp/p2p3P/B1kP4/P7/1P2N1P1/1K3N2 w - - 2 2");

        CompareMiniMax_AlphaBeta(fen, 1);


        //RunPerft(fen, 1, 1, 20);
        //RunPerft(fen, 2, 1, 400);
        //RunPerft(fen, 3, 1, 8902);
        //RunPerft(fen, 4, 1, 197281);
        //RunPerft(fen, 5, 1, 2826);
        //RunPerft(fen, 8, 1, 10000000000000);
        //RunPerft(fen, 2, 1,  195314821);
        //RunPerft(fen, 3, 1,  195314821);
        //RunPerft(fen, 4, 1,  195314821);
        //RunPerft(fen, 5, 1,  195314821);
        //RunPerft(fen, 6, 1,  195314821);
        //RunPerft(fen, 7, 1,  195314821);
        //RunPerft(fen, 9, 1,  195314821);
        //RunPerft(fen, 11, 1,  195314821);
    }
    if(0)
    {
        const std::string fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        //    Bitboard& b = BitboardFromFen(fen);
        //Show<ShowTypes::Console>::Op(evaluate(b));
        //RunPerft(fen, 1, 1, 48);
        //RunPerft(fen, 2, 1, 2039);
        //RunPerft(fen, 3, 1, 97862);
        //RunPerft(fen, 4, 1, 4085603);
        CompareMiniMax_AlphaBeta(fen, 4);
        //RunPerft(fen, 5, 1, 193690690);
        //RunPerft(fen, 6, 1, 8031647685);

    }
    if(RunPerfts)
    {
        const std::string fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        //RunPerft(fen, 1, 1, 14);
        //RunPerft(fen, 2, 1, 191);
        //RunPerft(fen, 3, 1, 2812);
        //RunPerft(fen, 4, 1, 43238);
        RunPerft(fen, 5, 1, 674624);
        //RunPerft(fen, 6, 1, 11030083);
    }
    if(RunPerfts)
    {
        const std::string fen("8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - - 1 67");
        //RunPerft(fen, 1, 1, 50);
        //RunPerft(fen, 2, 1, 279);
    }
    if(0)
    {
        const std::string fen("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");

        CompareMiniMax_AlphaBeta(fen, 12);
        //RunPerft(fen, 1, 1, 24);
        //RunPerft(fen, 2, 1, 496);
        //RunPerft(fen, 3, 1, 9483);
        //RunPerft(fen, 4, 1, 182838);
        //RunPerft(fen, 5, 1, 3605103);
        //RunPerft(fen, 6, 1, 71179139);
    }
    if(RunPerfts)
    {
        const std::string fen("rnbqkb1r/ppppp1pp/7n/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
        //RunPerft(fen, 5, 1, 11139762);
    }
    if(RunPerfts)
    {
        const std::string fen("8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28");
        //RunPerft(fen, 6, 1, 38633283);
    }

    //RunZobTest("4k3/8/8/8/8/8/4P3/4K3 w KQkq - 0 1", 10);
    //RunZobTest("r1bqkb1r/5ppp/p1np1n2/3Np1B1/1p2P3/NP6/P1P2PPP/R2QKB1R b KQkq - 0 11", 100);
    //RunZobTest("4k3/8/8/8/8/3p4/4P3/4K3 w - - 0 1", 10);

    return 0;
}
