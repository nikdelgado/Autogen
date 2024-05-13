#
# Simplistic sample Dockerfile
#

# Sample app has to work in AERO and BPN, so we are passing in url arg
# Your app probably doesn't need to do this
FROM registry-gitlab.com/common/images/ubi8:noupdate
ARG BSF_REPO_HOST

ARG PIP_INDEX_URL

COPY CAcerts.pem /etc/ssl/aero-cacert.pem
ENV PIP_CERT=/etc/ssl/aero-cacert.pem
ENV CURL_CA_BUNDLE=/etc/ssl/aero-cacert.pem


COPY requirements.txt /opt/build/
WORKDIR /opt/build

RUN dnf update -y && dnf install -y \
    automake \
    gcc \
    gcc-c++ \
    git \
    make \
    python39 \
    python39-pip \
    python39-devel 

RUN python3 -m pip install --upgrade pip

RUN set -x && \
    pip config set global.index-url $PIP_INDEX_URL && \
    pip install --upgrade pip && \
    pip install -r requirements.txt


# This image is just available to adhoc run the autogen tool with dependencies
CMD ["/bin/sh"]
