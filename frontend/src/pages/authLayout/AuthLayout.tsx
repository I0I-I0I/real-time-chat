import { Gradient } from "@/components/UI"
import { Outlet } from "react-router"

import styles from "./Auth.module.css"

const AuthLayout = () => {
    return (
        <>
            <Gradient />
            <div className={styles.wrapper}>
                <Outlet />
                <div className={styles.bg_image} />
            </div>
        </>
    )
}

export default AuthLayout
