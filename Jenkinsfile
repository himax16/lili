pipeline {
  agent any
  stages {
    stage('Setup Spack') {
      parallel {
        stage('Setup Spack') {
          steps {
            sh '''!#/bin/bash
pwd'''
          }
        }

        stage('Checkout Git') {
          steps {
            git(url: 'git@github.com:himax16/lili.git', branch: 'development', changelog: true)
          }
        }

      }
    }

    stage('Setup Build Environment') {
      steps {
        sh '''#!/bin/bash
pwd'''
      }
    }

    stage('Compile Program') {
      steps {
        sh '''!#/bin/bash
echo "Compiling LILI"'''
      }
    }

    stage('Regression Run - Test Particles') {
      steps {
        sh '''!#/bin/bash
echo "Running regression test using test particle problem"'''
      }
    }

  }
}