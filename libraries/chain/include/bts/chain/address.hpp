#pragma once

#include <bts/chain/config.hpp>
#include <bts/chain/pts_address.hpp>

#include <fc/array.hpp>
#include <fc/crypto/ripemd160.hpp>

namespace fc { namespace ecc {
    class public_key;
    typedef fc::array<char,33>  public_key_data;
} } // fc::ecc

namespace bts { namespace chain {

   struct public_key_type;

   /**
    *  @brief a 160 bit hash of a public key
    *
    *  An address can be converted to or from a base58 string with 32 bit checksum.
    *
    *  An address is calculated as ripemd160( sha512( compressed_ecc_public_key ) )
    *
    *  When converted to a string, checksum calculated as the first 4 bytes ripemd160( address ) is
    *  appended to the binary address before converting to base58.
    */
   class address
   {
      public:
       address(); ///< constructs empty / null address
       explicit address( const std::string& base58str );   ///< converts to binary, validates checksum
       address( const fc::ecc::public_key& pub ); ///< converts to binary
       explicit address( const fc::ecc::public_key_data& pub ); ///< converts to binary
       address( const pts_address& pub ); ///< converts to binary
       address( const public_key_type& pubkey );

       static bool is_valid( const std::string& base58str, const std::string& prefix = BTS_ADDRESS_PREFIX );

       explicit operator std::string()const; ///< converts to base58 + checksum

       fc::ripemd160 addr;
   };
   inline bool operator == ( const address& a, const address& b ) { return a.addr == b.addr; }
   inline bool operator != ( const address& a, const address& b ) { return a.addr != b.addr; }
   inline bool operator <  ( const address& a, const address& b ) { return a.addr <  b.addr; }

} } // namespace bts::chain

namespace fc
{
   void to_variant( const bts::chain::address& var,  fc::variant& vo );
   void from_variant( const fc::variant& var,  bts::chain::address& vo );
}

namespace std
{
   template<>
   struct hash<bts::chain::address>
   {
       public:
         size_t operator()(const bts::chain::address &a) const
         {
            return (uint64_t(a.addr._hash[0])<<32) | uint64_t( a.addr._hash[0] );
         }
   };
}

#include <fc/reflect/reflect.hpp>
FC_REFLECT( bts::chain::address, (addr) )
