c2amqp-adapter
---

The adapter that receives Ceph s3-compatible HTTP notifications and converts to
AMQP1.0.

Dependencies:

- Pistache, the http frontend, installed from ppa
- Qpid-proton-cpp, the amqp1.0 library, installed from official packages

The git submodules now are deprecated.

Usage:[this is deprecated, please follow the Dockerfile approach]
- install the pistache and qpid-proton library mannually
- clone this repo and use `git submodule update --init --recursive`
- `mkdir build && cd build && cmake .. && make`
- you can use run the `demo_run.sh` to quickly set up the demo, or
- `cd src`, run the broker, adapter, and simple_recv one by one, each
	should be run in individual terminal( this step can be replaced by the
	demo_run.sh )
- you can use ` curl -d '{"key1":"value1", "key2":"value2"}' -H "Content-Type:
	application/json" -X POST http://localhost:7074` to receive the Post-data in
	simple_recv

[Suggested method]:
Or you can set up this demo using the provided Dockerfile:
1. clone this repo
2. `docker build -t demo-adapter .`
3. docker run demo-adapter

or to better enter a dev environment, you can mount this folder in docker run
command below:
`docker run --mount type=bind,source=$SRC_PATH,target=$PATH -it demo-adapter`

[a simple workflow]
[![adaptor.png](https://i.postimg.cc/QdH2xWRV/adaptor.png)](https://postimg.cc/mPWXmhZG)

Todo:
- Error handling 
- Endpoint configuration for more alterable feature
