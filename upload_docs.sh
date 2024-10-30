#!/bin/bash
# Variables
GITHUB_USERNAME="himax16"
REPO_NAME="lili-docs"

BUILD_DOCS_DIRECTORY="build/docs/sphinx"
DOXYGEN_DIRECTORY="build/docs/doxygen/html"

# Copy doxygen files to the root directory
cp -r $DOXYGEN_DIRECTORY $BUILD_DOCS_DIRECTORY/doxygen

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
git remote add origin git@github.com:$GITHUB_USERNAME/$REPO_NAME.git

# Add and commit files
git add .
git commit -m "Push documentation"

# Force push to GitHub
git push -u origin master --force
