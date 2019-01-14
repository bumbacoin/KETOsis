pipeline {
  agent any
  stages {
    stage('Copy To Windows Build Location') {
      steps {
        sh '''rm -rf /mnt/KETOsis-Master
cp -a $WORKSPACE /mnt/KETOsis-Master
cd /mnt
cp compile-keto.sh KETOsis-Master/compile-keto.sh 
cd KETOsis-Master
qmake
make distclean 
'''
      }
    }
    stage('Leveldb builds') {
      parallel {
        stage('Linux') {
          steps {
            sh '''cd src/leveldb
chmod +x build_detect_platform
make libleveldb.a libmemenv.a
cd ../../'''
          }
        }
        stage('Windows') {
          steps {
            sh '''cd /mnt/KETOsis-Master/
export PATH=/mnt/mxe/usr/bin:$PATH
cd src/leveldb
chmod +x build_detect_platform
TARGET_OS=NATIVE_WINDOWS make libleveldb.a libmemenv.a CC=/mnt/mxe/usr/bin/i686-w64-mingw32.static-gcc CXX=/mnt/mxe/usr/bin/i686-w64-mingw32.static-g++
'''
          }
        }
      }
    }
    stage('Build Wallets') {
      parallel {
        stage('Linux QT') {
          steps {
            sh 'qmake && make -j5'
          }
        }
        stage('Linux Daemon') {
          steps {
            sh '''cd src/
make -j3 -f makefile.unix'''
          }
        }
        stage('Windows Wallet') {
          steps {
            sh '''export PATH=/mnt/mxe/usr/bin:$PATH
cd /mnt/KETOsis-Master
./compile-keto.sh'''
          }
        }
      }
    }
    stage('Create build Folder') {
      steps {
        sh '''cd /var/www/html/dir
mkdir -p $JOB_NAME/$BUILD_NUMBER
'''
      }
    }
    stage('Move Wallets') {
      parallel {
        stage('Move Linux QT') {
          steps {
            sh '''cp ketosis-qt /var/www/html/dir/$JOB_NAME/$BUILD_NUMBER/KETOsis-qt
'''
          }
        }
        stage('Move Linux Daemon') {
          steps {
            sh 'cp src/ketosisd /var/www/html/dir/$JOB_NAME/$BUILD_NUMBER/ketosisd'
          }
        }
        stage('Move Windows QT') {
          steps {
            sh 'cp /mnt/KETOsis-Master/release/ketosis-qt.exe /var/www/html/dir/$JOB_NAME/$BUILD_NUMBER/KETOsis-qt.exe'
          }
        }
      }
    }
    stage('Delete Project Folder') {
      steps {
        cleanWs(cleanWhenAborted: true, cleanWhenFailure: true, cleanWhenNotBuilt: true, cleanWhenSuccess: true, cleanWhenUnstable: true, cleanupMatrixParent: true, deleteDirs: true)
      }
    }
  }
}
