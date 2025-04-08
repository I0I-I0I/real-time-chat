import { Button, Link, Typography, FormInput } from "@/components/UI"

import styles from "../Auth.module.css"
import useInput from "@/hooks/useInput"
import UserService from "@/api/UserService"
import { useNavigate } from "react-router"
import { useUserStore } from "@/state/user"
import { useState } from "react"
import { IUser } from "@/types"
import cls from "@/utils/cls"

export const Login = (): JSX.Element => {
    const [invalidData, setInvalidData] = useState(false)
    const [invalidPassword, setInvalidPassword] = useState(false)
    const [login_prop,] = useInput("")
    const [password_prop,] = useInput("")
    const navigate = useNavigate()
    const setUserState = useUserStore((state) => state.setUser)
    const setUserAuthState = useUserStore((state) => state.setAuth)

    const handleSubmit = async (e: React.FormEvent<HTMLFormElement>) => {
        e.preventDefault()
        const response = await UserService.checkOne({
            login: login_prop.value,
            password: password_prop.value
        })
        if (!response) {
            setInvalidPassword(false)
            setInvalidData(true)
            return
        }
        if (response.message === "Wrong password") {
            setInvalidData(false)
            setInvalidPassword(true)
            return
        }
        const data = response.data[0] as IUser
        if (!data) return
        setUserState(data)
        setUserAuthState(true)
        navigate("/")
    }

    return (
        <div className={styles.login}>
            <Typography tag="h1" variant="title-1">Sign in</Typography>
            <form action="POST" className="form sing-in" onSubmit={handleSubmit}>
                <FormInput
                    className={invalidData ? styles.invalid : ""}
                    type="text"
                    placeholder="Login..."
                    invalidData={invalidData}
                    textOnInvalidData={"Invalid data"}
                    {...login_prop}
                />
                <FormInput
                    className={invalidData ? styles.invalid : ""}
                    type="password"
                    placeholder="Password..."
                    invalidData={invalidData || invalidPassword}
                    textOnInvalidData={invalidPassword ? "Wrong password" : "Invalid data"}
                    {...password_prop}
                />
                <Button
                    className={styles.button}
                    type="submit"
                    disabled={login_prop.value === "" || password_prop.value === ""}
                >Sing in</Button>
                <Link href="/register" className={cls(styles.link, styles.or_link)}>or sing up</Link>
            </form>
        </div>
    )
}
