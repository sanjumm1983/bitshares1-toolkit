#pragma once
#include <bts/db/object.hpp>

namespace bts { namespace chain {

   /**
    *  All operations and virtual operations result in the creation of an 
    *  operation_history_object that is maintained on disk as a stack.  Each
    *  real or virtual operation is assigned a unique ID / sequence number that
    *  it can be referenced by.   
    */
   class operation_history_object : public abstract_object<operation_history_object>
   {
      public:
         static const uint8_t space_id = protocol_ids;
         static const uint8_t type_id  = operation_history_object_type;;

         operation_history_object( const operation& o ):op(o){}
         operation_history_object(){}

         operation         op;
         operation_result  result;
         /** the block that caused this operation */
         uint32_t          block_num = 0;
         /** the transaction in the block */
         uint16_t          trx_in_block = 0;
         /** the operation within the transaction */
         uint16_t          op_in_trx = 0;
         /** any virtual operations implied by operation in block */
         uint16_t          virtual_op = 0;
   };

   /**
    *  Account history is important for users and wallets even though it is
    *  not part of "core validation".   Account history is maintained as
    *  a linked list stored on disk in a stack.  Each account will point to the
    *  most recent account history object by ID.  When a new operation relativent
    *  to that account is processed a new account history object is allcoated at
    *  the end of the stack and intialized to point to the prior object.  
    *
    *  This data is never accessed as part of chain validation and therefore
    *  can be kept on disk as a memory mapped file.  Using a memory mapped file
    *  will help the operating system better manage / cache / page files and
    *  also accelerates load time.
    *
    *  When the transaction history for a particular account is requested the
    *  linked list can be traversed with relatively effecient disk access because
    *  of the use of a memory mapped stack.
    */
   class account_transaction_history_object :  public abstract_object<account_transaction_history_object>
   {
      public:
         static const uint8_t space_id = implementation_ids;
         static const uint8_t type_id  = impl_account_transaction_history_object_type;
         operation_history_id_type            operation_id;
         account_transaction_history_id_type  next;
   };
} } // bts::chain

FC_REFLECT_DERIVED( bts::chain::operation_history_object, (bts::chain::object),
                    (op)(result)(block_num)(trx_in_block)(op_in_trx)(virtual_op) )

FC_REFLECT_DERIVED( bts::chain::account_transaction_history_object, (bts::chain::object), 
                    (operation_id)(next) )