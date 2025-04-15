import AuthService from "@/api/AuthService"
import { Button, FormInput, Link, Typography } from "@/components/UI"
import useInput from "@/hooks/useInput"
import { useUserStore } from "@/state/user"
import cls from "@/utils/cls"
import { useEffect, useState } from "react"
import { useNavigate } from "react-router"
import styles from "../Auth.module.css"

export const Login = (): JSX.Element => {
    const [invalidData, setInvalidData] = useState(false)
    const [invalidPassword, setInvalidPassword] = useState(false)
    const [login_prop,] = useInput("")
    const [password_prop,] = useInput("")
    const navigate = useNavigate()
    const setUserState = useUserStore((state) => state.setUser)
    const setUserAuthState = useUserStore((state) => state.setAuth)
    const isAuth = useUserStore((state) => state.auth)

    const handleSubmit = async (e: React.FormEvent<HTMLFormElement>) => {
        e.preventDefault()
        const response = await AuthService.loginWithPassword(login_prop.value, password_prop.value)
        if (!response) {
            setInvalidPassword(false)
            setInvalidData(true)
            return
        }
        if (response.status === 401) {
            setInvalidData(false)
            setInvalidPassword(true)
            return
        }
        const data = response.data[0]
        if (!data) return
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
            </form>
            <Link href="/register" className={cls(styles.link, styles.or_link)}>or sing up</Link>
        </div>
    )
}
