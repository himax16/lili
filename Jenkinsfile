pipeline {
  agent any
  stages {
    stage('Setup Spack') {
      steps {
        sh('echo "Setting up Spack"')
      }
    }

    stage('Setup Build Environment') {
      steps {
        sh('echo "Setting up build environment"')
      }
    }

    stage('Compile Program') {
      steps {
        sh('echo "Compiling program"')
      }
    }

    stage('Regression Run - Test Particles') {
      steps {
        sh('echo "Running regression test for particles"')
      }
    }

  }
}
