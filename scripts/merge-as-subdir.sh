#!/bin/bash

set -u # Don't allow executing with empty variable
set -e # Exit when command fails

REMOTE_NAME=$1
REMOTE_URL=$2
REMOTE_BRANCH=$3
REPO_PREFIX=$4

PREV_BRANCH=$(git rev-parse --abbrev-ref HEAD)

# Fetching
git remote add $REMOTE_NAME $REMOTE_URL
git fetch $REMOTE_NAME $REMOTE_BRANCH

git switch -c merge-$REMOTE_NAME
git merge -s ours --no-commit --allow-unrelated-histories $REMOTE_NAME/$REMOTE_BRANCH
git read-tree --prefix=$REPO_PREFIX -u $REMOTE_NAME/$REMOTE_BRANCH
git commit -m "Merge $REMOTE_NAME/$REMOTE_BRANCH as subdir"

git pull -s subtree $REMOTE_NAME $REMOTE_BRANCH

git checkout $PREV_BRANCH

echo "Manually rebase:"
echo "git rebase merge-$REMOTE_NAME"
echo "git branch -D merge-$REMOTE_NAME"
