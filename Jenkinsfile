pipeline {
  agent any
  environment {
    DISCORD_WEBHOOK = credentials('discord_hima_server')
  }
  stages {
    stage('Setup Spack') {
      steps {
        echo 'Setting up Spack'
      }
    }

    stage('Setup Build Environment') {
      steps {
        echo 'Setting up build environment'
      }
    }

    stage('Compile Program') {
      steps {
        echo 'Compiling program'
      }
    }

    stage('Regression Run - Test Particles') {
      steps {
        echo 'Running regression test for particles'
      }
    }

    stage('Clean Up') {
      steps {
        echo 'Done!'
        discordSend enableArtifactsList: true, showChangeset: true, webhookURL: "$DISCORD_WEBHOOK", link: env.BUILD_URL, result: currentBuild.currentResult, title: "${env.JOB_NAME} - #${env.BUILD_NUMBER}", description: "Build #${env.BUILD_NUMBER} of ${env.JOB_NAME} status: ${currentBuild.currentResult}"
      }
    }
  }
}
