(use base58)

(assert (= (encode "1") "r"))
(assert (= (encode "2") "s"))
(assert (= (encode "3") "t"))

(assert (= (checked_encode "1") "16Xzqssd"))
(assert (= (checked_encode "2") "16iLLyNC"))
(assert (= (checked_encode "3") "16pW9Ed5"))

(def msg "123456789abcdefghjklmnpqrstuvwxyzabcdefghijkmnopqrstuvwxyz")
(assert (= (decode (encode msg)) msg))

(def [result version] (checked_decode (checked_encode "abcd" 7)))
(assert (= result "abcd"))
(assert (= version 7))

(assert (= [msg 8] (checked_decode (checked_encode msg 8))))
