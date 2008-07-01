#ifndef AWS_S3_S3CONNECTION_API_H
#define AWS_S3_S3CONNECTION_API_H

#include <istream>
#include <libaws/common.h>

namespace aws {

  class S3Connection : public SmartObject
  {
    public:
      virtual ~S3Connection() {}

      /*! \brief Creates a bucket on S3
       *
       * This function creates a bucket on S3. The name of the bucket to create
       * is passed as a parameter to this function.
       *
       * @param aBucketName The name of the bucket to create.
       * @returns aws::s3::CreateBucketResponse containing information about the bucket that
       *          was created.
       *
       * \throws aws::s3::CreateBucketException if the bucket could not be created successfully.
       * \throws aws::AWSConnectionException if a connection error occured.
       */
      virtual CreateBucketResponsePtr
      createBucket(const std::string& aBucketName) = 0;

      /*! \brief Gets all buckets that belong to the caller's account
       *
       * This function receives all buckets that belong to the caller's AWS account.
       *
       * @returns aws::s3::ListAllBucketsResponsePtr containing a list of all buckets
       *          belonging to the caller's account.
       *
       * \throws aws::s3::ListAllBucketsException if the list of all buckets could not be received.
       * \throws aws::AWSConnectionException if a connection error occured.
       */
      virtual ListAllBucketsResponsePtr
      listAllBuckets() = 0;

      /*! \brief Deletes a bucket on S3
       *
       * This function deletes a bucket on S3. The name of the bucket to delete
       * is passed as a parameter to this function.
       *
       * @param aBucketName The name of the bucket to delete.
       * @returns aws::s3::DeleteBucketResponse containing information about the bucket that
       *          was delete.
       *
       * \throws DeleteBucketException 
       */
      virtual DeleteBucketResponsePtr
      deleteBucket(const std::string& aBucketName) = 0;

      /*! \brief Lists touples from a bucket
       *
       * This function lists touples from a bucket on S3. The name and some
       * parameters to configure the query are passed to this function.
       * Normally, not all queried tuples are returned. Thus, the response contains 
       * a marker that is the key of the last returned tuple. This mark can be used
       * to make an additional query for all tuples with are lexically located after
       * the marker.
       *
       * @param aBucketName The name of the bucket.
       * @param aPrefix Only tuples that have a key with this prefix are returned.
       * @param aMarker key of the last returned tuple, empty string is equivalent
       *                to no marker
       * @param aMaxKeys Maxium amount of returned tuples
       *
       * @returns aws::s3::ListBucketRespose
       *
       * \throws ListBucketException
       */
      virtual ListBucketResponsePtr
      listBucket(const std::string& aBucketName, 
                 const std::string& aPrefix, 
                 const std::string& aMarker, 
                 int aMaxKeys) = 0;

      /*! \brief Lists touples from a bucket
       *
       * This function lists touples from a bucket on S3. The name and some
       * parameters to configure the query are passed to this function.
       * Normally, not all queried tuples are returned. Thus, the response contains 
       * a marker that is the key of the last returned tuple. This mark can be used
       * to make an additional query for all tuples with are lexically located after
       * the marker.
       *
       * @param aBucketName The name of the bucket.
       * @param aPrefix Only tuples that have a key with this prefix are returned.
       * @param aMarker key of the last returned tuple, empty string is equivalent
       *                to no marker
       * @param aDelimiter Causes keys that contain the same string between the prefix 
       *                   and the first occurrence of the delimiter to be rolled up into
       *                   a single result element in the CommonPrefixes collection. 
       * @param aMaxKeys Maxium amount of returned tuples
       *
       * @returns aws::s3::ListBucketRespose
       *
       * \throws ListBucketException
       */
      virtual ListBucketResponsePtr
      listBucket(const std::string& aBucketName, 
                 const std::string& aPrefix, 
                 const std::string& aMarker, 
                 const std::string& aDelimiter,
                 int aMaxKeys) = 0;

      /*! \brief Put an object on S3.
       *
       * Stores an object given in an input stream on S3. The object is stored in the given bucket using the given key.
       *
       * @param aBucketName The name of the bucket the object should be stored in.
       * @param aKey The name of the key the object should be stored with.
       * @param aData The object to store as an input stream.
       * @param aContentType The content type of the object to store.
       * @param aSize An optional parameter specificying the size of the object.
       *              If -1 is passed, seek is used on the input stream to determine the size
       *              of the object to send.
       *
       * \throws aws::s3::PutException if the object couldn't be stored.
       * \throws aws::AWSConnectionException if a connection error occured.
       */
      virtual PutResponsePtr
      put(const std::string& aBucketName,
          const std::string& aKey,
          std::istream& aData,
          const std::string& aContentType,
          long aSize = -1) = 0;


      /*! \brief Put an object on S3.
       *
       * Stores an object given as a char pointer on S3. The object is stored in the given bucket using the given key.
       * 
       * @param aBucketName The name of the bucket the object should be stored in.
       * @param aKey The name of the key the object should be stored with.
       * @param aData The object to store as a char pointer.
       * @param aContentType The content type of the object to store.
       * @param aSize Parameter specificying the size of the object.
       *
       * \throws aws::s3::PutException if the object couldn't be stored
       * \throws aws::AWSConnectionException if a connection error occured.
       */
       virtual PutResponsePtr
       put(const std::string& aBucketName,
           const std::string& aKey,
           const char* aData,
           const std::string& aContentType,
           long aSize) = 0;

      /*! \brief Receive an object from S3.
       *
       * This function receives and object from S3. The object is retrieved from the
       * given bucket with the given key.
       *
       * @param aBucketName The name of the bucket in which the object is stored.
       * @param aKey The key for which the object should be retrieved.
       *
       * \throws aws::s3::GetException if the object coldn't be received.
       * \throws aws::AWSConnectionException if a connection error occured.
       */
      virtual GetResponsePtr
      get(const std::string& aBucketName,
          const std::string& aKey) = 0;

      /*! \brief Receive an object from S3 if the given ETag has changed.
       *
       * This function receives and object from S3. The object is only retrieved from the
       * given bucket with the given key if the ETag on S3 is different then the
       * given ETag.
       *
       * @param aBucketName The name of the bucket in which the object is stored.
       * @param aKey The key for which the object should be retrieved.
       * @param aOldETag The ETag of an object with the given key. The object
       *        is only retrieved if the ETag for the according object is different on
       *        S3.
       *
       * \throws aws::s3::GetException if the object coldn't be received.
       * \throws aws::AWSConnectionException if a connection error occured.
       */
      virtual GetResponsePtr
      get(const std::string& aBucketName,
          const std::string& aKey,
          const std::string& aOldEtag) = 0;

      /*! \brief Delete an object from S3. 
       *
       * This function delete an object in the given bucket with the given key from S3.
       *
       * @param aBucketName The name of the bucket in which the object is stored.
       * @param aKey The key for which the object to be deleted.
       *
       * \throws aws::s3::DeleteException if the object coldn't be deleted.
       * \throws aws::AWSConnectionException if a connection error occured.
       */
      virtual DeleteResponsePtr
      del(const std::string& aBucketName,
          const std::string& aKey) = 0;

      virtual HeadResponsePtr
      head(const std::string& aBucketName,
          const std::string& aKey) = 0;

  }; /* class S3Connection */

} /* namespace aws */
#endif
