FROM ubuntu AS builder

RUN apt update -y && DEBIAN_FRONTEND=noninteractive apt install -y git clang lld cmake make ruby
COPY . /workspace
WORKDIR /workspace
RUN git submodule update --checkout --init --recursive test/unit-test/CMock source/dependency/coreJSON source/dependency/3rdparty/tinycbor

RUN CC=clang CXX=clang++ LDFLAGS="-fuse-ld=lld -fprofile-instr-generate" cmake -S test -B build
RUN make -C build all


FROM ubuntu

COPY --from=builder /workspace/build/bin/*_fuzz /
