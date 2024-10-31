#!/bin/bash
# This script is used to upload the documentation to GitHub Pages

# Variables
DOCS_GITHUB_REPO="git@github.com:himax16/lili-docs.git"
BUILD_DOCS_DIRECTORY="build/docs/sphinx"

# Change directory to the build docs directory
cd $BUILD_DOCS_DIRECTORY

# Remove any existing git information
rm -rf .git

# Add .nojekyll file to the directory
touch .nojekyll

# Add CNAME file to the directory
echo "lili.hima.id" > CNAME

# Initialize a new git repository
git init
git remote add origin $DOCS_GITHUB_REPO

# Add and commit files
git add .
git commit -m "Push documentation"

# Force push to GitHub
git push -u origin master --force

# Remove any existing git information
rm -rf .git
