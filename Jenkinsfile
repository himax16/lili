pipeline {
  agent any
  stages {
    stage('Setup Spack') {
      steps {
        bash 'echo "Setting up Spack"'
      }
    }

    stage('Setup Build Environment') {
      steps {
        bash 'echo "Setting up build environment"'
      }
    }

    stage('Compile Program') {
      steps {
        bash 'echo "Compiling program"'
      }
    }

    stage('Regression Run - Test Particles') {
      steps {
        bash 'echo "Running regression test for particles"'
      }
    }

  }
}
