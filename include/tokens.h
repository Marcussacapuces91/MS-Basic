#pragma once

#include <ostream>
#include <string>
#include <regex>

class Token {
public:
    friend std::ostream& operator<<(std::ostream&, const Token&);

///< To distinguish between String or Number identifier (with $ terminator).
    enum type_t { FLOAT, INTEGER, STRING, CHANEL };


protected:
    virtual const std::string str() const = 0;
};

/**
 * TokenComment
 */
class TokenComment : public Token {
public:
/**
 * Constructor initializing text value.
 */
    TokenComment(const std::string& aText) : text(aText) {}

/**
 * Factory building the token from the passed text or returning a nullptr if none recognized.
 */
    static TokenComment* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop) {
        static const std::regex exp("^REM(.*)", std::regex_constants::icase);

        std::smatch sm;
        if (std::regex_match(aStart, aStop, sm, exp)) {
            aStart += sm[0].length();
            return new TokenComment(sm[1]);
        }
        return nullptr; // No instruction found!
    }

protected:
    virtual const std::string str() const {
        return "REM"+text;
    }

private:
///< Comment content.
    const std::string text;
};


/**
 * TokenInstruction
 */
class TokenInstruction : public Token {
public:
/**
 * Constructor initializing id value.
 */
    TokenInstruction(const unsigned aId) : id(aId) {}

/**
 * Factory building the token from the passed text or returning a nullptr if none recognized.
 */
    static TokenInstruction* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop) {
        for (unsigned i = 0; i < sizeof(tokens) / sizeof(tokens[0]) ; ++i) {
            const std::regex exp("^" + tokens[i] + ".*", std::regex_constants::icase);
            if (std::regex_match(aStart, aStop, exp)) {
                aStart += tokens[i].size();
                return new TokenInstruction(i);
            }
        }
        return nullptr; // No instruction found!
    }

protected:
    virtual const std::string str() const {
        return tokens[id];
    }

private:
///< Token id.
    const unsigned id;

///< List of all tokens allowed for instructions.
    static const std::string tokens[99];
};

const std::string TokenInstruction::tokens[] = {
    "AUTO",
    "BEEP", "BLOAD", "BSAVE",
    "CALL", "CHAIN", "CHDIR", "CIRCLE", "CLEAR", "CLOSE", "CLS", "COLOR", "COM", "COMMON", "CONT",
    "DATA", "DEF", "FNSEG", "FNUSR", "DELETE", "DIM", "DRAW",
    "EDIT", "ELSE", "END", "ERASE", "ERROR",
    "FIELD", "FILES", "FOR", "TO", "STEP",
    "GET", "GOSUB", "GOTO",
    "IF", "INPUT",
    "KEY", "KILL",
    "LET", "LINE", "LIST", "LLIST", "LOAD", "LOCK", "LPRINT", "LSET",
    "MERGE", "MKDIR",
    "NAME", "NEXT", "NEW",
    "ON", "COM", "PLAY", "STRIG", "TIMER", "OPEN", "OPTION_BASE", "OUT",
    "PAINT", "PALETTE", "PEEK", "PEN", "PLAY", "PMAP", "POINT", "POKE", "PRESET", "PRINT", "PSET", "PUT",
    "RANDOMIZE", "READ", "RENUM", "RESET", "RESTORE", "RESUME", "RETURN", "RMDIR", "RSET", "RUN",
    "SAVE", "SCREEN", "SHELL", "SOUND", "STOP", "STRIG", "SYSTEM",
    "THEN", "TROFF", "TRON",
    "UNLOCK",
    "WAIT", "WEND", "WHILE", "WIDTH", "WINDOW", "WRITE"
};

/**
 * TokenIdentifier
 */
class TokenIdentifier : public Token {
public:
/**
 * Constructor.
 */
    TokenIdentifier(const std::string& aName, const type_t& aType) : name(aName), type(aType) {}

/**
 * Factory.
 */
    static TokenIdentifier* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop) {
        static const std::regex exp("^([A-Z]+\\w*[\\$%]?).*", std::regex_constants::icase);

        std::smatch sm;
        if (std::regex_match(aStart, aStop, sm, exp)) {
            aStart += sm[1].length();
            const type_t t = sm[1].str()[sm[1].length() - 1] == '$' ? STRING : (sm[1].str()[sm[1].length() - 1] == '%' ? INTEGER : FLOAT);
            return new TokenIdentifier(sm[1], t);
        }
        return nullptr; // No identifier found!
    }

    const std::string& getName() const {
        return name;
    }

    const type_t& getType() const {
        return type;
    }

protected:
    virtual const std::string str() const {
        return name;
    }

private:
    const std::string name;
    const type_t type;
};

/**
 * TokenOperator
 */
class TokenOperator : public Token {
public:
/**
 * Constructor.
 */
    TokenOperator(const std::string& aId) : id(aId) {}

/**
 * Factory.
 */
    static TokenOperator* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop) {
        static const std::regex exp("^([+\\-*\\/<>=()\\[\\]\\%\\^]+).*", std::regex_constants::icase);

        std::smatch sm;
        if (std::regex_match(aStart, aStop, sm, exp)) {
            aStart += sm[1].length();
            return new TokenOperator(sm[1]);
        }
        return nullptr; // No identifier found!
    }

protected:
    virtual const std::string str() const {
        return id;
    }

private:
    const std::string id;
};

/**
 * TokenConstant
 */
class TokenConstant : public Token {
public:
/**
 * Constructor.
 */
    TokenConstant(const std::string& aId, const type_t& aType) : id(aId), type(aType) {}

/**
 * Factory.
 */
    static TokenConstant* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop) {
        static const std::regex expInt("^(\\d+).*");
        static const std::regex expFloat("^(\\d+([.]\\d*)?(e[+-]?\\d+)?|[.]\\d+(e[+-]?\\d+)?).*", std::regex_constants::icase);
        static const std::regex expString("^\"(.*?[^\\\\])\".*$");
        static const std::regex expChanel("^(#\\d+).*$");

        std::smatch sm;

        if (std::regex_match(aStart, aStop, sm, expFloat)) {
            aStart += sm[1].length();
            return new TokenConstant(sm[1], FLOAT);
        }
        if (std::regex_match(aStart, aStop, sm, expString)) {
            aStart += sm[1].length() + 2; // 2 quotes char.
            return new TokenConstant(sm[1], STRING);
        }
        if (std::regex_match(aStart, aStop, sm, expChanel)) {
            aStart += sm[1].length();
            return new TokenConstant(sm[1], CHANEL);
        }
        if (std::regex_match(aStart, aStop, sm, expInt)) {
            aStart += sm[1].length();
            return new TokenConstant(sm[1], INTEGER);
        }
        return nullptr; // No identifier found!
    }

protected:
    virtual const std::string str() const {
        return (type == STRING ? '"' + id + '"' : id);
    }

private:
    const std::string id;
    const type_t type;
};

/**
 * TokenSeparator
 */
class TokenSeparator : public Token {
public:
/**
 * Constructor.
 */
    TokenSeparator(const std::string& aId) : id(aId) {}

/**
 * Factory.
 */
    static TokenSeparator* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop) {
        static const std::regex exp("^([:;,]).*");

        std::smatch sm;
        if (std::regex_match(aStart, aStop, sm, exp)) {
            aStart += sm[1].length();
            return new TokenSeparator(sm[1]);
        }
        return nullptr; // No token found!
    }

protected:
    virtual const std::string str() const {
        return id;
    }

private:
    const std::string id;
};

/**
 * Operator<< for token instances.
 */
std::ostream& operator<<(std::ostream& out, const Token& t) { return out << t.str(); }

std::ostream& operator<<(std::ostream& out, const std::vector<Token*> list) {
    for (auto it = list.begin(); it != list.end(); ++it) {
        out << **it << " ";
    }
    return out;
}
