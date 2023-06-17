# Introducing VIZ

![Build docker images](https://github.com/VIZ-Blockchain/viz-cpp-node/workflows/Build%20docker%20images/badge.svg)

VIZ is a Graphene blockchain with a Fair-DPOS consensus algorithm (vote weight splitted to selected witnesses, witness gets a penalty for missing a block).

## Building

See [documentation/building.md](documentation/building.md) for detailed build instructions, including
compile-time options, and specific commands for Linux (Ubuntu LTS) or macOS X.

## Running in docker

Auto-built image [vizblockchain/vizd](https://hub.docker.com/r/vizblockchain/vizd) is available at docker hub.

Docker image tags:

* **latest** - built from master branch, used to run production VIZ network
* **testnet** - built from master branch, could be used to run [local test network](documentation/testnet.md)

Example run:

```
docker run \
    -d -p 2001:2001 -p 8090:8090 -p 8091:8091 --name vizd \
    vizblockchain/vizd:latest

docker logs -f vizd
```

## Seed Nodes

A list of some seed nodes to get you started can be found in
[share/vizd/seednodes](share/vizd/seednodes).

This same file is baked into the docker images and can be overridden by
setting `VIZD_SEED_NODES` in the container environment at `docker run`
time to a whitespace delimited list of seed nodes (with port).

## Building docker images manually

Production:

```
docker build -t viz:latest -f share/vizd/docker/Dockerfile-production .
```

Testnet + keep intermediate build containers for debugging:

```
docker build --rm=false -t viz:testnet -f share/vizd/docker/Dockerfile-testnet .
```
