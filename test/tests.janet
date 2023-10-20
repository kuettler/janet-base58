(use base58)

(assert (= (encode "1") "r"))
(assert (= (encode "2") "s"))
(assert (= (encode "3") "t"))

(assert (= (checked-encode "1") "16Xzqssd"))
(assert (= (checked-encode "2") "16iLLyNC"))
(assert (= (checked-encode "3") "16pW9Ed5"))

(def msg "123456789abcdefghjklmnpqrstuvwxyzabcdefghijkmnopqrstuvwxyz")
(assert (= (decode (encode msg)) msg))

(def [result version] (checked-decode (checked-encode "abcd" 7)))
(assert (= result "abcd"))
(assert (= version 7))

(assert (= [msg 8] (checked-decode (checked-encode msg 8))))
