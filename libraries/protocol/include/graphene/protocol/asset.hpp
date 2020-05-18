#pragma once

#include <graphene/protocol/types.hpp>
#ifdef BUILD_TESTNET
#include <graphene/protocol/config_testnet.hpp>
#else
#include <graphene/protocol/config.hpp>
#endif

namespace graphene { namespace protocol {

        typedef uint64_t asset_symbol_type;

        struct asset {
            asset(share_type a = 0, asset_symbol_type id = TOKEN_SYMBOL)
                    : amount(a), symbol(id) {
            }

            share_type amount;
            asset_symbol_type symbol;

            double to_real() const {
                return double(amount.value) / precision();
            }

            uint8_t decimals() const;

            std::string symbol_name() const;

            int64_t precision() const;

            void set_decimals(uint8_t d);

            static asset from_string(const string &from);

            string to_string() const;

            asset &operator+=(const asset &o) {
                FC_ASSERT(symbol == o.symbol);
                amount += o.amount;
                return *this;
            }

            asset &operator-=(const asset &o) {
                FC_ASSERT(symbol == o.symbol);
                amount -= o.amount;
                return *this;
            }

            asset& operator*=(int x) {
                amount *= x;
                return *this;
            }
            asset& operator/=(int x) {
                amount /= x;
                return *this;
            }
            asset operator*(int x) const {
                return asset(amount * x, symbol);
            }
            asset operator/(int x) const {
                return asset(amount / x, symbol);
            }

            asset operator-() const {
                return asset(-amount, symbol);
            }

            friend bool operator==(const asset &a, const asset &b) {
                FC_ASSERT(a.symbol == b.symbol);
                return a.amount == b.amount;
            }

            friend bool operator<(const asset &a, const asset &b) {
                FC_ASSERT(a.symbol == b.symbol);
                return a.amount < b.amount;
            }

            friend bool operator<=(const asset &a, const asset &b) {
                return (a == b) || (a < b);
            }

            friend bool operator!=(const asset &a, const asset &b) {
                return !(a == b);
            }

            friend bool operator>(const asset &a, const asset &b) {
                return !(a <= b);
            }

            friend bool operator>=(const asset &a, const asset &b) {
                return !(a < b);
            }

            friend asset operator-(const asset &a, const asset &b) {
                FC_ASSERT(a.symbol == b.symbol);
                return asset(a.amount - b.amount, a.symbol);
            }

            friend asset operator+(const asset &a, const asset &b) {
                FC_ASSERT(a.symbol == b.symbol);
                return asset(a.amount + b.amount, a.symbol);
            }

            friend asset operator*(int l, const asset& r) {
                return r * l;
            }

        };

        struct price {
            price(const asset &base = asset(), const asset quote = asset())
                    : base(base), quote(quote) {
            }

            asset base;
            asset quote;

            static price max(asset_symbol_type base, asset_symbol_type quote);

            static price min(asset_symbol_type base, asset_symbol_type quote);

            price max() const {
                return price::max(base.symbol, quote.symbol);
            }

            price min() const {
                return price::min(base.symbol, quote.symbol);
            }

            double to_real() const {
                return base.to_real() / quote.to_real();
            }

            bool is_null() const;

            void validate() const;
        };

        price operator/(const asset &base, const asset &quote);

        inline price operator~(const price &p) {
            return price{p.quote, p.base};
        }

        bool operator<(const asset &a, const asset &b);

        bool operator<=(const asset &a, const asset &b);

        bool operator<(const price &a, const price &b);

        bool operator<=(const price &a, const price &b);

        bool operator>(const price &a, const price &b);

        bool operator>=(const price &a, const price &b);

        bool operator==(const price &a, const price &b);

        bool operator!=(const price &a, const price &b);

        asset operator*(const asset &a, const price &b);
        inline asset operator*(const price& l, const asset& r) {
            return r * l;
        }

} } // graphene::protocol

namespace fc {
    inline void to_variant(const graphene::protocol::asset &var, fc::variant &vo) {
        vo = var.to_string();
    }

    inline void from_variant(const fc::variant &var, graphene::protocol::asset &vo) {
        vo = graphene::protocol::asset::from_string(var.as_string());
    }
}

FC_REFLECT((graphene::protocol::asset), (amount)(symbol))
FC_REFLECT((graphene::protocol::price), (base)(quote))
