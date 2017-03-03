#pragma once

//
// Implements a Fraction class w/ full operator overloading,
// and simple algebraic parser. 
// Has static unit tests that use assert(); to run, call Fraction::unittests().
//

#ifndef FRACTION_DISPLAY_AS_MIXED_FRACTIONS
    #define FRACTION_DISPLAY_AS_MIXED_FRACTIONS 1
#endif
#ifndef FRACTION_NO_UNITTESTS
    #define FRACTION_NO_UNITTESTS 0
#endif
#ifndef FRACTION_NO_STRING_PARSING
    #define FRACTION_NO_STRING_PARSING 0
#endif

#include <algorithm>
#define gcd std::__gcd


// #include <numeric>      // std::gcd in Fraction
#include <ostream>      // std::ostream in Fraction
#include <istream>      // std::istream in Fraction + FractionParser
#include <string>       // std::string in Fraction + FractionParser

#if !FRACTION_NO_UNITTESTS
    #include <cassert>      // assert in Fraction::unittests()
#endif
#if !FRACTION_NO_STRING_PARSING
    #include <sstream>      // std::stringstream in FractionParser
    #include <vector>       // std::vector in FractionParser
#endif

#include <iostream>

struct Fraction {
    int    n;
    int    d;

    Fraction (int n = 0, int d = 1) : n(n), d(d) {
        if (this->d < 0) { this->d = -this->d; this->n = -this->n; }
        if (this->d > 1) {
            auto cf = gcd(abs(this->n), this->d);
            assert(cf >= 1);
            if (cf > 1) {
//                std::cout << "Reducing " << this->n << "/" << this->d;
                this->n /= cf;
                this->d /= cf;
//                std::cout << " to " << this->n << "/" << this->d << '\n';
            }
        }
    }
    // Default copy-ctor
    Fraction (const Fraction&) = default;

    // From-string ctor
    Fraction (const std::string& s);

    auto& operator= (const Fraction& other) {
        n = other.n;
        d = other.d;
        return *this;
    }

    #define DEFN_CMP(op) \
    friend bool operator op (const Fraction& a, const Fraction& b) { \
        return (a.n * b.d) op (b.n * a.d); \
    }
    DEFN_CMP(==)
    DEFN_CMP(!=)
    DEFN_CMP(>)
    DEFN_CMP(<)
    DEFN_CMP(>=)
    DEFN_CMP(<=)
    #undef DEFN_CMP

    friend Fraction operator+ (const Fraction& a, const Fraction& b) {
        return { a.n * b.d + b.n * a.d, a.d * b.d };
    }
    friend Fraction operator* (const Fraction& a, const Fraction& b) {
        return { a.n * b.n, a.d * b.d };
    }
    friend Fraction operator- (const Fraction& a, const Fraction& b) {
        return a + Fraction { -b.n, b.d };
    }
    friend Fraction operator/ (const Fraction& a, const Fraction& b) {
        return a * Fraction { b.d, b.n };
    }

    auto& operator += (const Fraction& other) { return *this = (*this + other); }
    auto& operator -= (const Fraction& other) { return *this = (*this - other); }
    auto& operator *= (const Fraction& other) { return *this = (*this * other); }
    auto& operator /= (const Fraction& other) { return *this = (*this / other); }

    auto& operator ++ () { return *this += { +1, 1 }; }
    auto& operator -- () { return *this += { -1, 1 }; }

    friend auto& operator << (std::ostream& os, const Fraction& f) {
        if (f.d == 0)
            return os << (f.n >= 0 ? "+inf" : "-inf");

        if (FRACTION_DISPLAY_AS_MIXED_FRACTIONS) {

            int q = f.n / f.d;
            int r = f.n % f.d;
            
            if (r == 0 || f.n == 0 || f.d == 1)
                return os << f.n;
            if (q == 0)
                return os << r << "/" << f.d;
            return os << q << "+" << r << "/" << f.d;

        } else {
            if (f.d == 1 || f.n == 0)
                return os << f.n;
            return os << f.n << "/" << f.d;
        }
    }
#if !FRACTION_NO_STRING_PARSING
    friend auto& operator >> (std::istream& is, Fraction& f) {
        return f = /*FractionParser::*/parse(is), is;
    }
    static Fraction fromStr (const std::string s) {
        return /*FractionParser::*/parse(s);
    }
#endif // !FRACTION_NO_STRING_PARSING
    
    // friend auto& operator >> (std::istream& is, Fraction& f) {
        // int n = 0, r = 0; unsigned d = 1; char c = '\0';
        // int[4] inf_s;

        // if (is && is.get(inf_s[0])) {
        //     int i = 0; bool ok = true;
        //     if (inf_s[i] == '+' || inf_s[i] == '-') {
        //         ok = is.get(inf_s[++i]);
        //     }

        //     if (ok = ok && c == 'i') {
        //         ok = ok && is.get(inf_s[++i]) && inf_s[i] == 'n';
        //         ok = ok && is.get(inf_s[++i]) && inf_s[i] == 'f';
        //     }
        //     if (!ok) {
        //         while (i) is.unget(inf_s[i--]);
        //     } else {

        //     }
        // }

        // if (is) is >> n;
        // if (is) is.get(c);
        // while (isspace(c)) is.get(c);
        // swtich (c) {
        //     case '+': 
        //         while (is.get(c) && isspace(c));
        //         if (c != '+') is.unget(c);
        //         is >> r;
        //         if (is && is.get(c))
        //             while (isspace(c)) is.get(c);
        //         if (c != '/')
        //             break;
        //     case '/':
        //         while (is.get(c) && isspace(c));
        //         if (c != '+') is.unget(c);
        //         is >> d;
        //         break;
        //     default:
        // }
        // return f = { (n * d + r), d }, is;        
    // }

    std::string str () {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

#if !FRACTION_NO_UNITTESTS
    template <typename A, typename B>
    static void assertEq (const A& a, const B& b) {
        if (a != b)
            std::cerr << "Assertion Failure: " << a << " != " << b << '\n';
    }
    template <typename A, typename B>
    static void assertNe (const A& a, const B& b) {
        if (a == b)
            std::cerr << "Assertion Failure: " << a << " == " << b << '\n';
    }
    template <typename Exception, typename F>
    static auto assertThrows (const F& f, bool shouldThrow = true) {
        try {
            f();
            if (shouldThrow)
                std::cerr << "Assertion Failure: expected thrown exception " << '\n';
        } catch (const Exception* e) {
            if (!shouldThrow)
                std::cerr << "Assertion Failure: expected no exception, got " << e->what() << '\n';
        } catch (const ParseException* e) {
            if (shouldThrow)
                std::cerr << "Assertion Failure: wrong exception thrown" << '\n';
        } catch (std::exception* e) {
            if (shouldThrow)
                std::cerr << "Assertion Failure: wrong exception thrown" << '\n';
        }
    }

    #define RUN_TEST(test) std::cout << "\n\nRunning Test: " #test "\n"; test()

    static void unittest () {
        RUN_TEST(unittest_enforce);
        RUN_TEST(unittest_skipSpace);
        RUN_TEST(unittest_parseNumber);
        RUN_TEST(unittest_resolveExpr);
        RUN_TEST(unittest_flushOps);
        RUN_TEST(unittest_parseExpr);
        RUN_TEST(unittest_Fraction);
    }
    static void unittest_Fraction () {
        assertEq(Fraction{ 4, 0 }, Fraction{ 9, 0 });
        assertEq(Fraction{ 0, 4 }, Fraction{ 0, 9 });

        assertEq(Fraction{ 1, 1 }, Fraction { 1, 1 });
        assertNe(Fraction{ -1, 1 }, Fraction{ 1, 1 });
        assertEq(Fraction{ 8, 4 }, Fraction{ 2, 1 });
        
        assertEq(Fraction{ 1, 1 } + Fraction { 1, 2 }, Fraction { 3, 2 });

        assertEq(Fraction{ 4, 6 }.n, 2);
        assertEq(Fraction{ 4, 6 }.d, 3);
        assertEq(Fraction{ 4, 6 }.str(), "2/3");
        assertEq(Fraction("2/3"), Fraction(2,3));
        
        assertEq(Fraction(1,0).str(), "+inf");
        assertEq(Fraction(-1,0).str(), "-inf");
        assertEq(Fraction(0,0).str(), "+inf");
        assertEq(Fraction("+inf").str(),"+inf");
        assertEq(Fraction("-inf").str(),"-inf");
        assertEq(Fraction("inf").str(),"+inf");
        assertEq(Fraction("+inf"),Fraction(22,0));
        assertNe(Fraction(-22,0),Fraction(22,0));
        assertEq(Fraction(12312,0),Fraction(123,0));
    }    
#endif // !FRACTION_NO_UNITTESTS
// };

#if !FRACTION_NO_STRING_PARSING

// Fraction expression parser; implements subset of a generalized algebraic expression.
// Features + limitations:
//  – supports + - * / ( ) w/ correct order of operations
//  – numbers: decimal (0-9) + 'inf' _only_; supports signs.
//  – does not support symbols or functions.
//  - no support for boolean comparison operations.
//  – all operators assumed to be binary + left associative.
// 
// Implementation details:
//  – Throws non-descriptive ParseExceptions when/if hits any syntax errors; no pointer is provided.
//  - Built around std::istream (deliberately) as an experiment. Yes, we know it's slow / non-optimal:
//    this parser is built for simplicity + use of standard library data structures, NOT speed.
//    No, we wouldn't use this in real-world scenarios.
//
// struct FractionParser {
    static Fraction parse (std::istream& is) {
        std::stringstream ss { '(' };
        ss << is.rdbuf();
        ss << ')';
        return parseExpr(ss);
    }
    static Fraction parse (const std::string& s) {
        std::stringstream ss; assert(ss.good());
        ss << '(';
        ss << s; assert(ss.good());
        ss << ')' << std::endl; assert(ss.good());
        return parseExpr(ss);
    }

    struct ParseException : public std::runtime_error {
        ParseException (const char* what) : std::runtime_error(what) {}
    };

    #define DECL_EXCEPTION(Cls,what) \
    struct Cls : public ParseException { \
        Cls () : ParseException(what) {} \
    };
    DECL_EXCEPTION(ExpectedNumber,   "expected number")
    DECL_EXCEPTION(ExpectedExpr,     "expected expression")
    DECL_EXCEPTION(UnbalancedRParen, "unbalanced ')'")
    DECL_EXCEPTION(UnbalancedExpr,   "unbalanced expr")
    DECL_EXCEPTION(InvalidOp,        "invalid op")
    DECL_EXCEPTION(IntegerOverflow,  "integer overflow")
    #undef DECL_EXCEPTION

protected:
    template <typename Ex, typename T>
    static void enforce (const T& condition) {
        if (!condition)
            throw new Ex();
    }
    static void unittest_enforce () {
        assertThrows<ExpectedNumber>([](){ enforce<ExpectedNumber>(false); }, true);
        assertThrows<ExpectedNumber>([](){ enforce<ExpectedNumber>(true); }, false);
    }

    template <typename Ex>
    static void skipSpace (std::istream& is, char& c) {
        while (isspace(c))
            enforce<Ex>((bool)is.get(c));
    }
    static void unittest_skipSpace () {
        std::stringstream ss { "f  a\t w\tl\nab  \n\tl " };
        char c = ss.peek();             assertEq(c, 'f');  assertEq((char)ss.peek(), 'f');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'f');  assertEq((char)ss.peek(), 'f');
        ss.get(c);                      assertEq(c, 'f');  assertEq((char)ss.peek(), ' ');
        ss.get(c);                      assertEq(c, ' ');  assertEq((char)ss.peek(), ' ');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'a');  assertEq((char)ss.peek(), '\t');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'a');  assertEq((char)ss.peek(), '\t');
        ss.get(c);                      assertEq(c, '\t'); assertEq((char)ss.peek(), ' ');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'w');  assertEq((char)ss.peek(), '\t');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'w');  assertEq((char)ss.peek(), '\t');
        ss.get(c);                      assertEq(c, '\t'); assertEq((char)ss.peek(), 'l');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'l');  assertEq((char)ss.peek(), '\n');
        ss.get(c);                      assertEq(c, '\n'); assertEq((char)ss.peek(), 'a');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'a');  assertEq((char)ss.peek(), 'b');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'a');  assertEq((char)ss.peek(), 'b');
        ss.get(c);                      assertEq(c, 'b');  assertEq((char)ss.peek(), ' ');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'b');  assertEq((char)ss.peek(), ' ');
        ss.get(c);                      assertEq(c, ' ');  assertEq((char)ss.peek(), ' ');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'l');  assertEq((char)ss.peek(), ' ');
        skipSpace<ExpectedExpr>(ss, c); assertEq(c, 'l');  assertEq((char)ss.peek(), ' ');
        ss.get(c);                      assertEq(c, ' ');  assertEq((char)ss.peek(), '\377');
        assertThrows<ExpectedExpr>([&](){ skipSpace<ExpectedExpr>(ss, c); }); //assertEq(c, '\377');
        assertThrows<ExpectedExpr>([&](){ skipSpace<ExpectedExpr>(ss, c); }); //assertEq(c, '\377');
    }

    static Fraction parseNumber (std::istream& is) {
        char c;

        enforce<ExpectedNumber>(is && is.get(c));            
        skipSpace<ExpectedNumber>(is, c);

        // Handle optional sign
        int sign = 1;
        if (c == '+') {
            enforce<ExpectedNumber>((bool)is.get(c));
        } else if (c == '-') {
            enforce<ExpectedNumber>((bool)is.get(c));
            sign = -1;
        }
        // std::cout << "sign = " << sign;

        if (c == 'i') {     // "inf" case
            enforce<ExpectedNumber>(is.get(c) && c == 'n');
            enforce<ExpectedNumber>(is.get(c) && c == 'f');
            // std::cout << " inf\n";
            return Fraction { sign, 0 };
        }

        // Otherwise, expect decimal numbers.
        enforce<ExpectedNumber>(isdigit(c));

        int n = (c - '0');
        // std::cout << " [" << c << "," << n << "]";
        while (is.get(c) && isdigit(c)) {
            n *= 10;
            n += (c - '0');
            enforce<IntegerOverflow>(n >= 0);
            // std::cout << "[" << c << "," << n << "]";
        }
        // std::cout << " n = " << n << '\n';
        
        if (!isdigit(c))
            is.unget();

        return Fraction { sign * n, 1 };
    }
    static void unittest_parseNumber () {
        std::stringstream ss { "1 +2 -3 123 456 78912301 inf -inf +inf -1209414212 -120941421241241 "}; char c;
        assertEq(parseNumber(ss).str(), "1"); //assert(ss.get(c) && c == ' ');
        assertEq(parseNumber(ss).str(), "2"); //assert(ss.get(c) && c == ' ');
        assertEq(parseNumber(ss).str(), "-3"); //assert(ss.get(c) && c == ' ');
        assertEq(parseNumber(ss).str(), "123"); //assert(ss.get(c) && c == ' ');
        assertEq(parseNumber(ss).str(), "456"); //assert(ss.get(c) && c == ' ');
        assertEq(parseNumber(ss).str(), "78912301"); //assert(ss.get(c) && c == ' ');
        assertEq(parseNumber(ss).str(), "+inf");
        assertEq(parseNumber(ss).str(), "-inf");
        assertEq(parseNumber(ss).str(), "+inf");
        assertEq(parseNumber(ss).str(), "-1209414212");
        assertThrows<IntegerOverflow>([&](){ assertEq(parseNumber(ss).str(), "-120941421241241"); });
    }

    static Fraction parseExpr (std::istream& is) {

        std::vector<Fraction>   terms;
        std::vector<int>        ops;

        #define GET_OP(v)           (v & 0xff)
        #define GET_PRED(v)         (v >> 16)
        #define MAKE_OP(op, pred)   (op | (pred << 16))
        
        assert(is.good());

        char c;
        int parenDepth = 0;
        while (1) {
            enforce<ExpectedExpr>(is.get(c));
            skipSpace<ExpectedExpr>(is, c);

            switch (c) {
                case '(': ++parenDepth; ops.push_back(MAKE_OP('(', 3)); break;
                case ')':
                    enforce<UnbalancedRParen>(--parenDepth >= 0);
                    while (enforce<UnbalancedRParen>(ops.size() != 0), GET_OP(ops.back()) != '(') {
                        resolveExpr(terms, ops);
                    }
                    ops.pop_back();
                    if (parenDepth == 0)
                        return terms.size() ? terms[0] : Fraction{ 0, 0 };
                    
                    if (ops.size() == 0 || parenDepth == 0) {
                        enforce<UnbalancedExpr>(terms.size() == 1);
                        return terms[0];
                    }
                    break;
                case '+':
                    if (terms.size() == 0) { is.unget(); terms.push_back(parseNumber(is)); }
                    else                   { flushOps(terms, ops, '+', 2); }
                    break;
                case '-':
                    if (terms.size() == 0) { is.unget(); terms.push_back(parseNumber(is)); }
                    else                   { flushOps(terms, ops, '-', 2); }
                    break;
                case '*': flushOps(terms, ops, '*', 1); break;
                case '/': flushOps(terms, ops, '/', 1); break;
                default:
                    is.unget();
                    terms.push_back(parseNumber(is));
            }
        }
    }
    static void unittest_parseExpr () {
        std::stringstream ss { "(-inf) (1)  (1+1)  (2*2)  (3/2)  (4+2+4)  (2/2+3)  (2/3-4/5)  (1-1/4)  ((1+2)/(3+4))" };
        assertEq(parseExpr(ss).str(), "-inf");
        assertEq(parseExpr(ss).str(), "1");
        assertEq(parseExpr(ss).str(), "2");
        assertEq(parseExpr(ss).str(), "4");
        assertEq(parseExpr(ss).str(), "1+1/2");
        assertEq(parseExpr(ss).str(), "10");
        assertEq(parseExpr(ss).str(), "4");
        assertEq(parseExpr(ss).str(), "-2/15");
        assertEq(parseExpr(ss).str(), "3/4");
        assertEq(parseExpr(ss).str(), "3/7");
    }

    static void resolveExpr (std::vector<Fraction>& terms, std::vector<int>& ops) {
        assert(GET_OP(ops.back()) != '(');

        enforce<ExpectedExpr>(terms.size() >= 2);
        auto rhs = terms.back(); terms.pop_back();
        auto lhs = terms.back(); terms.pop_back();
        auto op  = ops.back(); ops.pop_back();

        switch (GET_OP(op)) {
            case '+': terms.push_back(lhs + rhs); break;
            case '-': terms.push_back(lhs - rhs); break;
            case '*': terms.push_back(lhs * rhs); break;
            case '/': terms.push_back(lhs / rhs); break;
            default: enforce<InvalidOp>(false);
        }
    }
    static void unittest_resolveExpr () {
        // TBD
    }

    static void flushOps (std::vector<Fraction>& terms, std::vector<int>& ops, int op, int pred) {
        while (ops.size() && GET_PRED(ops.back()) <= pred && GET_OP(ops.back()) != '(') {
            resolveExpr(terms, ops);
        }
        ops.push_back(MAKE_OP(op, pred));
    }
    static void unittest_flushOps () {
        // TBD
    }
};

Fraction::Fraction (const std::string& s) : n(0), d(0) {
    try {
        *this = Fraction::parse(s);
    } catch (Fraction::ParseException* e) {
        std::cerr << "PARSE ERROR: " << e->what() << " in '" << s << "'\n";
    }
}

#endif // !FRACTION_NO_STRING_PARSING
