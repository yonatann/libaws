/*
 * Copyright 2008 28msec, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <iostream>
#include <sstream>
#include <libaws/aws.h>

using namespace aws;

int
createQueue(SQSConnection* lSQSCon)
{
  {
    try {
      CreateQueueResponsePtr lCreateQueue = lSQSCon->createQueue("testQueue");
      std::cout << "Bucket created successfully. RequestId: " << lCreateQueue->getRequestId() << std::endl;
    } catch (CreateBucketException& e) {
      std::cerr << "Couldn't create bucket" << std::endl;
      std::cerr << e.what() << std::endl;
      return 1;
    }
  }
  return 0;
}

int
sqstest(int argc, char** argv) 
{
  
  AWSConnectionFactory* lFactory = AWSConnectionFactory::getInstance();

  std::cout << "Testing libaws version " << lFactory->getVersion() << std::endl;

  char* lAccessKeyId = getenv("AWS_ACCESS_KEY");
  char* lSecretAccessKey = getenv("AWS_SECRET_ACCESS_KEY");

  if (lAccessKeyId == 0 || lSecretAccessKey == 0) {
    std::cerr << "Environment variables (i.e. AWS_ACCESS_KEY or AWS_SECRET_ACCESS_KEY) not set" 
        << std::endl;
    return 1;
  }
  
  SQSConnectionPtr lS3Rest = lFactory->createSQSConnection(lAccessKeyId, lSecretAccessKey);

  int lReturnCode;
  try {
    lReturnCode = createQueue(lS3Rest.get());
    if (lReturnCode != 0)
      return lReturnCode;


  } catch (AWSConnectionException& e) {
    std::cerr << e.what() << std::endl;
    return 2;
  }

  lFactory->shutdown();

  return 0;
}