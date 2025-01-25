#!/bin/bash

cmake --preset=linux && cmake --build .

cmake --preset=windows && cmake --build .
