# Introducing VIZ

VIZ is a DPOS blockchain with an unproven consensus algorithm (Fair DPOS with witness penalty for missing block).

## Building

See [documentation/building.md](documentation/building.md) for detailed build instructions, including
compile-time options, and specific commands for Linux (Ubuntu LTS) or macOS X.

## Running in docker

Just want to get up and running quickly?  Try deploying a prebuilt
dockerized container.  Both common binary types are included.
To run a p2p node (ca. 2GB of memory is required at the moment):

    docker run \
        -d -p 2001:2001 -p 8090:8090 --name viz-default \
        viz-world/viz-world

    docker logs -f viz-default  # follow along

## Seed Nodes

A list of some seed nodes to get you started can be found in
[share/vizd/seednodes](share/vizd/seednodes).

This same file is baked into the docker images and can be overridden by
setting `VIZD_SEED_NODES` in the container environment at `docker run`
time to a whitespace delimited list of seed nodes (with port).
