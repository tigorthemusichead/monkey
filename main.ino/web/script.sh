#!/bin/bash

python .build.py
git checkout auto-develop
git add .
echo "Enter commit's name"
read commit_name 
git commit -m "$commit_name"
git push origin auto-develop
