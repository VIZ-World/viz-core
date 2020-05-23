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

To run a testnet node, please use [Docker](https://docs.docker.com/engine/install/).

To build image locally, use Dockerfile-testnet:

```
docker build -t viz:testnet -f share/vizd/docker/Dockerfile-testnet .
docker run -d --name vizd viz:testnet
docker logs --tail 100 -f vizd
```

To use pre-built image, run node with

```
docker run -d --name vizd vizblockchain/vizd:testnet
```

## Additional users

To ease testing, several additional users are immediately available:

* alice
* bob
* charlie
* chuck
* dan
* frank

All users have the same keys:

* private: 5J9DBCRX5D2ZUUuy9qV2ef9p5sfA3ydHsDs2G531bob7wbEigDJ
* public: VIZ5zTAE2hiGcqYaDTQeEBqTtPeoWtSNjpznwmbvqJXesrK1Qn3e8
