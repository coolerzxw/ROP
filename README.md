# ROP

ROP, or Resource Operating Protocol, is a request-response (i.e. half-duplex) protocol on top of TCP, used for client/server interactions.

Each request contains the following parts:
- Header
  - Length of Operation
  - Length of Resource
  - Length of Detail
- Operation
- Resource
- Detail

Each response contains the following parts:
- Header
  - Error code
  - Length of Result
- Result
