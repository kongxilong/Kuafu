FROM kuafu/kuafu:dev-18.04-x86_64-20200509_1417

RUN apt-get update && \
  apt-get install -y zsh 

# redis
WORKDIR /tmp
RUN wget http://release.fabu.ai/deps/redis-5.0.7.tar.gz && \
  tar xvzf redis-5.0.7.tar.gz && \
  cd redis-stable && \
  make -j12 && make install && \
  rm -rf /tmp/*


WORKDIR /Kuafu


