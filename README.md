# ROP

ROP, or Resource Operating Protocol, is a request-response (i.e. half-duplex) protocol on top of TCP, used for client/server interactions.

Each request contains the following parts:
- Header
  - Length of Operation (16-bit integer)
  - Length of Resource (16-bit integer)
  - Length of Detail (32-bit integer)
- Operation (length < 64KB)
- Resource (length < 64KB)
- Detail (length < 4GB)

Each response contains the following parts:
- Header
  - Error code (32-bit integer)
  - Length of Result (32-bit integer)
- Result (length < 4GB)
