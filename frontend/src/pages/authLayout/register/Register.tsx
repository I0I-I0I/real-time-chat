import { Button, Link, Typography, FormInput } from "@/components/UI"

import styles from "../Auth.module.css"
import useInput from "@/hooks/useInput"
import { useNavigate } from "react-router"
import { useUserStore } from "@/state/user"
import { useState, useEffect } from "react"
import cls from "@/utils/cls"
import AuthService from "@/api/AuthService"

export const Register = (): JSX.Element => {
    const [isInvalidData, setIsInvalidData] = useState(false)
    const [isUniqueLogin, setIsUniqueLogin] = useState(false)
    const [login_prop,] = useInput("")
    const [username_prop,] = useInput("")
    const [password_prop,] = useInput("")
    const navigate = useNavigate()
    const setUserState = useUserStore((state) => state.setUser)
    const setUserAuthState = useUserStore((state) => state.setAuth)
    const isAuth = useUserStore((state) => state.auth)

    const handleSubmit = async (e: React.FormEvent<HTMLFormElement>) => {
        e.preventDefault()
        setIsUniqueLogin(false)
        setIsInvalidData(false)
        const response = await AuthService.register({
            login: login_prop.value,
            username: username_prop.value,
            password: password_prop.value
        })
        if (!response) {
            setIsInvalidData(true)
            return
        }
        if (response.status === 422) {
            setIsUniqueLogin(true)
            return
        }
        const data = response.data[0]
        setUserState(data)
        if (data.hash) {
            localStorage.setItem("token", data.hash)
        }
        setUserAuthState(true)
        navigate("/")
    }

    useEffect(() => {
        if (isAuth) {
            navigate("/");
        }
    }, [isAuth, navigate]);

    if (isAuth) {
        return <div></div>;
    }

    return (
        <div className={styles.login}>
            <Typography tag="h1" variant="title-1">Sign up</Typography>
            <form action="POST" className="form sing-in" onSubmit={handleSubmit}>
                <FormInput
                    className={isInvalidData ? styles.invalid : ""}
                    type="text"
                    placeholder="Login..."
                    invalidData={isInvalidData || isUniqueLogin}
                    textOnInvalidData={isUniqueLogin ? "Login already exists" : "Invalid data"}
                    {...login_prop}
                />
                <FormInput
                    className={isInvalidData ? styles.invalid : ""}
                    type="text"
                    placeholder="Username..."
                    invalidData={isInvalidData}
                    textOnInvalidData="Invalid data"
                    {...username_prop}
                />
                <FormInput
                    className={isInvalidData ? styles.invalid : ""}
                    type="password"
                    placeholder="Password..."
                    invalidData={isInvalidData}
                    textOnInvalidData="Invalid data"
                    {...password_prop}
                />
                <Button
                    className={styles.button}
                    type="submit"
                    disabled={login_prop.value === "" || username_prop.value === "" || password_prop.value === ""}
                >Sing up</Button>
            </form>
            <Link href="/login" className={cls(styles.link, styles.or_link)}>or sing in</Link>
        </div>
    )
}
