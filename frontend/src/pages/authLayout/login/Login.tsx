import { Input, Modal, Button, Link, Typography } from "@/components/UI"

import styles from "../Auth.module.css"
import useInput from "@/hooks/useInput"
import UserService from "@/api/UserService"
import { useNavigate } from "react-router"
import { useUserStore } from "@/state/user"

export const Login = (): JSX.Element => {
	const [login_prop,] = useInput("")
	const [password_prop,] = useInput("")
    const navigate = useNavigate()
    const setUserState = useUserStore((state) => state.setUser)
    const setUserAuthState = useUserStore((state) => state.setAuth)

	const handleSubmit = async (e: React.FormEvent<HTMLFormElement>) => {
		e.preventDefault()
        const data = await UserService.checkOne({
            login: login_prop.value,
            password: password_prop.value
        })
        if (!data) return
        setUserState(data.login, data.username)
        setUserAuthState(true)
        navigate("/")
	}

	return (
        <Modal className={styles.modal_login}>
            <Typography tag="h1" variant="title-1">Sing in</Typography>
            <form action="POST" className="form sing-in" onSubmit={handleSubmit}>
                <Input
                    className="input login__input"
                    type="text"
                    placeholder="Login..."
                    {...login_prop}
                />
                <Input
                    className="input login__input"
                    type="password"
                    placeholder="Password..."
                    {...password_prop}
                />
                <Button className={styles.button} type="submit">Sing in</Button>
                <Link href="/register" className={styles.link}>or sing up</Link>
            </form>
        </Modal>
	)
}
