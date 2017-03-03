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

#include <numeric>      // std::gcd in Fraction
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


struct Fraction {
    int         n;
    unsigned    d;

    Fraction (decltype(n) n = 0, decltype(d) d = 1) : n(n), d(d) {
        auto cf = std::gcd(n, d);
        if (cf) {
            n /= cf;
            d /= cf;
        }
    }
    // Default copy-ctor
    Fraction (const Fraction&) = default;

    // From-string ctor
    Fraction (const std::string& s) : n(0), d(1) {
        std::stringstream { s } >> *this;
    }

    auto& operator= (const Fraction& other) {
        n = other.n;
        d = other.d;
        return *this;
    }

    #define DEFN_CMP(op) \
    friend bool operator cmp (const Fraction& a, const Fraction& b) {
        return (a.n * b.d) op (b.n * a.d);
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
        return a + { -b.n, b.d };
    }
    friend Fraction operator/ (const Fraction& a, const Fraction& b) {
        return a * { b.d, b.n };
    }

    auto& operator += (const Fraction& other) { return *this = (*this + other); }
    auto& operator -= (const Fraction& other) { return *this = (*this - other); }
    auto& operator *= (const Fraction& other) { return *this = (*this * other); }
    auto& operator /= (const Fraction& other) { return *this = (*this / other); }

    auto& operator ++ (const Fraction& other) { return *this += { +1, 1 }; }
    auto& operator -- (const Fraction& other) { return *this += { -1, 1 }; }

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
        return f = FractionParser::parse(is), is;
    }
    static Fraction fromStr (const std::string s) {
        return FractionParser::parse(s);
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
    static void unittest () { 
        assert(Fraction{ 4, 0 } == Fraction{ 9, 0 });
        assert(Fraction{ 0, 4 } == Fraction{ 0, 9 });

        assert(Fraction{ 1, 1 } == Fraction { 1, 1 });
        assert(Fraction{ -1, 1 } != Fraction{ 1, 1 });
        assert(Fraction{ 8, 4 } == Fraction{ 2, 1 });
        
        assert(Fraction{ 1, 1 } + Fraction { 1, 2 } == Fraction { 3, 2 });

        assert({Fraction{ 4, 6 }.str() == "2 / 3");
    }    
#endif // !FRACTION_NO_UNITTESTS
};

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
struct FractionParser {
    static void parse (std::istream& is) {
        std::stringstream ss { '(' };
        is >> ss;
        ss << ')';
        return parseExpr(ss);
    }
    static void parse (std::string& s) {
        std::stringstream ss { '(' };
        ss << s;
        ss << ')';
        return parseExpr(ss);
    }

    struct ParseException : public std::runtime_error {
        ParseException (const char* what) : std::runtime_error(what) {}
    };

    #define DECL_EXCEPTION(Cls,what) \
    struct Cls : public ParseException { \
        Cls () : std::ParseException(what) {} \
    };
    DECL_EXCEPTION(ExpectedNumber,   "expected number")
    DECL_EXCEPTION(ExpectedExpr,     "expected expression")
    DECL_EXCEPTION(UnbalancedRParen, "unbalanced ')'")
    DECL_EXCEPTION(UnbalancedExpr,   "unbalanced expr")
    #undef DECL_EXCEPTION

protected:
    template <typename Ex>
    static void expect (bool condition) {
        if (!condition)
            throw new Ex();
    }

    template <typename Ex>
    static void skipSpace (std::istream& is, int& c) {
        while (isspace(c))
            enforce<Ex>(is.get(c));
    }

    static Fraction parseNumber (std::istream& is) {
        int c;

        enforce<ExpectedNumber>(is && is.get(c));            
        skipSpace<ExpectedNumber>(is, c);

        // Handle optional sign
        int sign = 1;
        if (c == '+') {
            enforce<ExpectedNumber>(is.get(c));
        } else if (c == '-') {
            enforce<ExpectedNumber>(is.get(c));
            sign = -1;
        }

        if (c == 'i') {     // "inf" case
            enforce<ExpectedNumber>(is.get(c) && c == 'n');
            enforce<ExpectedNumber>(is.get(c) && c == 'f');
            return Fraction { sign, 0 };
        }

        // Otherwise, expect decimal numbers.
        enforce<ExpectedNumber>(isdigit(c));

        int n = c;
        while (is.get(c) && isdigit(c)) {
            n = n * 10 + c;
        }
        if (!isdigit(c))
            is.unget(c);

        return Fraction { sign * n, 1 };
    }
    static Fraction parseExpr (std::istream& is) {

        std::vector<Fraction>   terms;
        std::vector<int>        ops;

        #define GET_OP(v)           (v & 0xff)
        #define GET_PRED(v)         (v >> 16)
        #define MAKE_OP(op, pred)   (op | (pred << 16))

        int c;
        while (1) {
            enforce<ExpectedExpr>(is.get(c));
            skipSpace<ExpectedExpr>(is, c);

            switch (c) {
                case '(': ops.push_back(MAKE_OP('(', 1)); break;
                case ')': 
                    while (enforce<UnbalancedRParen>(ops.size() != 0), ops.back() != '(') {
                        resolveExpr(terms, ops);
                    }
                    ops.pop_back();
                    if (ops.size() == 0) {
                        enforce<UnbalancedExpr>(terms.size() == 1);
                        return terms[0];
                    }
                    break;
                case '+': flushOps(terms, ops, '+', 1); break;
                case '-': flushOps(terms, ops, '-', 1); break;
                case '*': flushOps(terms, ops, '*', 1); break;
                case '/': flushOps(terms, ops, '/', 1); break;
                default:
                    unget(c);
                    parseNumber(is);
            }
        }
    }
    static void resolveExpr (std::vector<Fraction>& terms, std::vector<int>& ops) {
        if (GET_OP(ops.back()) == '(')
            return;

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
    static void flushOps (std::vector<Fraction>& terms, std::vector<int>& ops, int op, int pred) {
        while (ops.size() && GET_PRED(ops.back()) <= pred) {
            resolveExpr(terms, ops);
        }
        ops.push_back(MAKE_OP(op, pred));
    }
};

#endif // !FRACTION_NO_STRING_PARSING
