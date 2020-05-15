# How to run a test network

When viz node starts without seed nodes, it tries to open a *snapshot.json* file which holds data on initial users and their balances.
If found, liquid tokens are transferred from `CHAIN_INITIATOR_NAME` account to users accounts into vesting (shares).

Initial users and their keys are defined inside *libraries/protocol/include/graphene/protocol/config.hpp*

* `CHAIN_INITIATOR_NAME` ("viz") is a user which holds initial balance.
* `CHAIN_COMMITTEE_ACCOUNT` is an initial active witness.

*viz* keys:

* private: 5JabcrvaLnBTCkCVFX5r4rmeGGfuJuVp4NAKRNLTey6pxhRQmf4
* public: VIZ6MyX5QiXAXRZk7SYCiqpi6Mtm8UbHWDFSV8HPpt7FJyahCnc2T

*committee* keys:

* private: 5Hw9YPABaFxa2LooiANLrhUK5TPryy8f7v9Y1rk923PuYqbYdfC
* public: VIZ6Yt7d6LsngBoXQr47aLv97bJVs7jyr7esZTM4UUSpLUf3nbRKS

## Launch

To run a testnet node, please use Dockerfile-testnet.
