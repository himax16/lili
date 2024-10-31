#!/bin/bash
# This script is used to upload the documentation to GitHub Pages

# Variables
DOCS_BUILD_DIR="build/docs/sphinx"
DOCS_GITHUB_REPO="git@github.com:himax16/lili-docs.git"
DOCS_CNAME="lili.hima.id"

# Change directory to the build docs directory
cd $DOCS_BUILD_DIR

# Remove any existing git information
rm -rf .git

# Add .nojekyll file to the directory
touch .nojekyll

# Add CNAME file to the directory
echo $DOCS_CNAME > CNAME

# Initialize a new git repository
git init
git remote add origin $DOCS_GITHUB_REPO

# Add and commit files
git add .
git commit -m "Update documentation"

# Force push to GitHub
git push -u origin master --force

# Remove any existing git information
rm -rf .git
