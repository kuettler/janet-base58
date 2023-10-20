(declare-project
  :name "base58"
  :description ```Base 58 encoding/decoding of buffers.```
  )

(def cflags '[])
(def lflags '["-lcrypto"])

(declare-native
  :name "base58"
  :source @["module.c" "base58.c"]
  :cflags [;default-cflags ;cflags]
  :lflags [;default-lflags ;lflags]
  )
