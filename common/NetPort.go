package testnet

import (
    "fmt"
    "io/ioutil"
    "strconv"
    "os"
    "log"
)

func GetEnvPort() int{
    data, err := ioutil.ReadFile("/tmp/obotcha_test_suit_port.txt")
    if err != nil {
        fmt.Println("File reading error", err)
        return 2001
    }
    int5, err := strconv.Atoi(string(data))
    return int5
}

func SetEnvPort(port int) {
    if err := ioutil.WriteFile("/tmp/obotcha_test_suit_port.txt", []byte(strconv.Itoa(port)), 0666); err != nil{
        os.Exit(111)
        log.Println(err.Error())
    }
}

func ResetEnvPort() {
    SetEnvPort(2001);
}