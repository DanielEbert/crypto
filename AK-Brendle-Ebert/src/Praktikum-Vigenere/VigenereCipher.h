#ifndef _VigenereCipher_h_
#define _VigenereCipher_h_

#include "ClassicCipher.h"

/**
 * @class A class for the affine cipher.
 * @author Christoph Karg
 *
 * A class for the  Vigenere  cipher.
 */
class VigenereCipher : public ClassicCipher {
public:
  /**
   * Decrypt a cipher text.
   *
   * @param cipher_text array to store cipher text
   * @param cipher_len length of the cipher text array
   * @param key key
   * @param key_len key length
   * @param plain_text plain text to be decrypted
   * @param plain_len length of the plain text
   *
   * @return number of decrypted bytes
   */
  virtual int decrypt(const vector<byte>& cipher_text, 
                      const vector<byte>&  key,
                      vector<byte>& plain_text);

   /**
   * Encrypt a plain text.
   *
   * @param plain_text plain text to be encrypted
   * @param plain_len length of the plain text
   * @param key key
   * @param key_len key length
   * @param cipher_text array to store cipher text
   * @param cipher_len length of the cipher text array
   *
   * @return number of encrypted bytes
   */
  virtual int encrypt( const vector<byte>& plain_text,
                       const vector<byte>& key, 
                       vector<byte>& cipher_text);

}; // VigenereCipher

#endif
