#ifndef __opening_book_creator_hpp
#define __opening_book_creator_hpp

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include "move.hpp"
#include "fen_parser.hpp"
#include "bitboard_fen.hpp"
#include "strings.hpp"

namespace OpeningBook
{
    class Appender : public std::vector<std::string> 
    {
    public:
        void operator() (const char* s, unsigned length) 
        {
            this->push_back(std::string(s,length));
        }
    };

    template<typename Operator>
    inline void tokenize(Operator& op, const char* input, const char* delimiters) 
    {
        const char* s = input;
        const char* e = s;
        while (*e != 0) 
        {
            e = s;
            while (*e != 0 && strchr(delimiters, *e) == 0) ++e;
            if (e - s > 0) 
            {
                op(s, (unsigned)(e - s));
            }
            s = e + 1;
        }
    }

    typedef std::string String;
    typedef std::vector<String> Strings;
    typedef std::vector<Appender> Appenders;

    struct Position_Option
    {
        Position_Option():mv(), score(0.0f), games_played(0){}
        move mv;
        float score;
        unsigned games_played;
        static std::string convert_to_string(const Position_Option& po)
        {
            std::string s, d("#");
            s += Sq::SSq[po.mv.from];
            s += Sq::SSq[po.mv.to];
            if(po.mv.special == MoveType::promotion)
            {
                std::string p("");
                if (po.mv.promoted == PieceType::wbishops || po.mv.promoted == PieceType::bbishops) p += "B";
                if (po.mv.promoted == PieceType::wqueens  || po.mv.promoted == PieceType::bqueens ) p += "Q";
                if (po.mv.promoted == PieceType::wknights || po.mv.promoted == PieceType::bknights) p += "N";
                if (po.mv.promoted == PieceType::wrooks   || po.mv.promoted == PieceType::brooks  ) p += "R";
                s += p;
            }
            s += d;
            s += to_string(po.score);
            s += d;
            s += to_string(po.games_played);
            return s;
        }
    };

    struct BookElement
    {
        std::string fen;
        ui64 key;
        std::vector<Position_Option> options;
    };

    struct TempBookElement
    {
        ui64 key;
        move mv;
    };

    struct RemoveUptoCharacter
    {
        RemoveUptoCharacter( char c ):ch(c){}
        void operator()(std::string& s) const
        {
            s = s.substr( s.find_first_of(ch) + 1 );
        }
        char ch;
    };

    struct KeyComparer
    {
        KeyComparer(ui64 aKey):key(aKey){}
        bool operator()(const BookElement& be) const { return be.key == key; }
        const ui64 key;
    };

    struct FenComparer
    {
        FenComparer(String aFen):fen(aFen){}
        bool operator()(const BookElement& be) const { return be.fen == fen; }
        const String fen;
    };

    struct MoveComparer
    {
        MoveComparer(const move& m):mv(m){}
        bool operator()(const Position_Option& po) const { return po.mv == mv; }
        const move& mv;
    };

    struct ToFrom
    {
        Sq::ESq from;
        Sq::ESq to;
    };

    struct file_writer
    {
        file_writer(std::ofstream& afile):file(afile){}
        void operator()(const String& s)
        {
            file << s << std::endl;
        }
        std::ofstream& file;
    };



    struct ConvertToStringAndWriteToFile
    {
        ConvertToStringAndWriteToFile(std::ofstream& afile):file(afile){}
        void operator()(const BookElement& be) const
        {
            String s(""), d1(";"), d2("%");
            if(!be.fen.empty())
            {
                s += be.fen;
                s += d1;
            }
            s += to_string(be.key);
            s += d1;
            for(unsigned i=0; i<be.options.size(); ++i)
            {
                s += Position_Option::convert_to_string(be.options[i]);
                s += d2;
            }
            s.pop_back();
            file << s << std::endl;
        }
        std::ofstream& file;
    };

    ToFrom to_from_string( const String& s )
    {
        std::string sf, st;

        sf += s[0];
        sf += s[1];

        st += s[2];
        st += s[3];

        ToFrom tf;

        for(unsigned i=0; i<64; ++i)
        {
            if(sf == Sq::SSq[i])
            {
                tf.from = (Sq::ESq)i;
            }
            if(st == Sq::SSq[i])
            {
                tf.to = (Sq::ESq)i;
            }
        }

        return tf;
    }

    bool GetMove( const ToFrom& tf, const String& s, type_array<move, Constants::max_moves_per_position>& mvs, move& mv, Bitboard& bb )
    {
        mvs.clear();
        GeneratePseudoLegalMoves(bb, mvs);

        if( s.length() > 4 )
        {
            PieceType::EPieceType p = PieceType::wqueens;
            switch(s[4])
            {
            case 'Q':
                p = PieceType::wqueens;
                break;
            case 'R':
                p = PieceType::wrooks;
                break;
            case 'B':
                p = PieceType::wbishops;
                break;
            case 'N':
                p = PieceType::wknights;
                break;
            }
            for(unsigned int i=0; i<mvs.size(); ++i)
            {
                if((mvs[i].from == tf.from) && (mvs[i].to == tf.to) && (p == mvs[i].promoted))
                {
                    mv = mvs[i];
                    return true;
                }
            }                
        }
        else
        {
            for(unsigned int i=0; i<mvs.size(); ++i)
            {
                if((mvs[i].from == tf.from) && (mvs[i].to == tf.to))
                {
                    mv = mvs[i];
                    return true;
                }
            }   
        }

        return false;
    }

    struct OpeningBookCreator
    {
        OpeningBookCreator(const String& fen, const String& database, const String& outFile)
            : bb(BitboardFromFen(fen)), dbFile(database), outputFile(outFile)
        {}

        void Create()
        {
            std::ifstream file(dbFile);
            String line;
            Appender game;
            game.reserve(100);
            //appenders.clear();
            //appenders.reserve(100000);
            bb.zobrists.push_back( ZobristFromBitboard(bb) );
            type_array<move, Constants::max_moves_per_position> mvs;

            while (std::getline(file, line))
            {
                game.clear();
                mvs.clear();

                tokenize(game, line.c_str(), " ");
                std::for_each(game.begin(), (game.end()-1), RemoveUptoCharacter('.'));

                float score = 0.0f;
                if     ( game.back() == "+1.0" ) score =  1.0f;
                else if( game.back() == "-1.0" ) score = -1.0f;
                else score = 0.0f;

                for(unsigned ii = 0; (ii < game.size()-1) && (ii < 40); ++ii )
                {
                    TempBookElement be;
                    be.key = bb.zobrists.back();
                    ToFrom tf = to_from_string(game[ii]);
                    move mv;
                    if(GetMove(tf, game[ii], mvs,  mv, bb))
                    {
                        be.mv = mv;
                        std::string fen("");
                        fen = FenFromBitboard(bb);
                        if(bb.MakeMove(mv))
                        {
                            bb.zobrists.push_back(bb.zobrists.back());
                            UpdateZobristFromMove(bb.zobrists.back(), mv, bb);
                            std::vector<BookElement>::iterator iter = std::find_if(book.begin(), book.end(), KeyComparer(be.key));
                            if( iter != book.end() )
                            {
                                BookElement& bookElem = (*iter);
                                std::vector<Position_Option>::iterator poi = std::find_if(bookElem.options.begin(), bookElem.options.end(), MoveComparer(be.mv));
                                if( poi != bookElem.options.end() )
                                {
                                    Position_Option& position_option = *(poi);
                                    ++(position_option.games_played);
                                    position_option.score += score;
                                }
                                else
                                {
                                    Position_Option po;
                                    po.mv = be.mv;
                                    po.games_played = 1;
                                    po.score = score;
                                    bookElem.options.push_back(po);
                                }
                            }
                            else
                            {
                                Position_Option po;
                                po.mv = be.mv;
                                po.games_played = 1;
                                po.score = score;

                                BookElement book_element;
                                book_element.key = be.key;
                                book_element.options.push_back(po);
                                book_element.fen = fen;

                                book.push_back(book_element);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                while(bb.moves.size() != 0)
                {
                    bb.UnmakeMove();
                    bb.zobrists.pop_back();
                }
                //appenders.push_back(game);
            }

            std::cout << book.size() << std::endl;

        }

        void WriteBookToFile( const String& filename ) const
        {
            std::ofstream out_file3(filename);
            std::for_each(book.begin(), book.end(), ConvertToStringAndWriteToFile(out_file3));
            out_file3.close();
        }

        Bitboard& bb;
        String dbFile;
        String outputFile;
        //Appenders appenders;
        std::vector<BookElement> book;
    };

    namespace MainTags
    {
        enum EMainTags
        {
            eFEN, eKey, eOptions
        };
    }

    namespace Position_Option_Tags
    {
        enum EPosition_Option_Tags
        {
            eMove, eScore, eNumGames
        };
    }

    
    Position_Option GetOption(const String& option_string, Bitboard& bb)
    {
        Position_Option po;
        Appender pos_option_tags;
        type_array<move, Constants::max_moves_per_position> mvs;

        tokenize(pos_option_tags, option_string.c_str(), "#");

        String mv_s = pos_option_tags[Position_Option_Tags::eMove];

        ToFrom tf = to_from_string(mv_s);
        bool found = GetMove(tf, mv_s, mvs, po.mv, bb);
        po.games_played = atoi(pos_option_tags[Position_Option_Tags::eNumGames].c_str());
        po.score = atoi(pos_option_tags[Position_Option_Tags::eScore].c_str());

        return po;
    }

    std::vector<Position_Option> GetOptions( String& options, String& fen )
    {
        Appender options_list;
        std::vector<Position_Option> options_vec;
        tokenize(options_list, options.c_str(), "%");
        Bitboard& bb = BitboardFromFen(fen);
        for(unsigned i=0; i<options_list.size(); ++i)
        {
            options_vec.push_back( GetOption(options_list[i], bb )); 
        }
        return options_vec;
    }

    ui64 atoui64(const char *szUnsignedInt) 
    {
        return _strtoui64(szUnsignedInt, NULL, 10);
    }

    struct OpeningBookLoader
    {
        OpeningBookLoader(const String& a_opening_book_file):opening_book_file(a_opening_book_file){}

        void Load()
        {
            std::ifstream file(opening_book_file);
            String line;

            while (std::getline(file, line))
            {
                Appender main_tags;
                tokenize(main_tags, line.c_str(), ";");
                BookElement be;
                be.fen = main_tags[MainTags::eFEN];
                be.key = atoui64(main_tags[MainTags::eKey].c_str());
                be.options = GetOptions(main_tags[MainTags::eOptions], be.fen);
                book.push_back(be);
            }
        }

        bool Query(ui64 key, move& mv) const
        {
            std::vector<BookElement>::const_iterator it = std::find_if(book.cbegin(), book.cend(), KeyComparer(key));
            String fen("rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2");
            std::vector<BookElement>::const_iterator it2 = std::find_if(book.cbegin(), book.cend(), FenComparer(fen));
            if(it == book.cend())
            {
                return false;
            }
            else
            {
                const BookElement& be = *it;
                unsigned i=0;
                unsigned games = 0;
                for(unsigned k=0; k<be.options.size(); ++k)
                {
                    if(be.options[k].games_played > games)
                    {
                        games = be.options[k].games_played;
                        i = k;
                    }
                }
                mv = be.options[i].mv;
                return true;
            }
        }

        std::vector<BookElement> book;
        const String opening_book_file;
    };

}


#endif